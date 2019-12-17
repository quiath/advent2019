
DEBUG = False

HALT = (99,)
AWAIT = (3,)

def decode(x):
    s = "{:05}".format(x)
    opcode = int(s[3:])
    params = tuple(int(y) for y in s[:3])
    #print("===", s, params)
    return (opcode,) + params



class Proc:
    def __init__(self, a, my_id):
        self.a = dict((i, a[i]) for i in range(len(a)))
        self.id = my_id
        self.ip = 0
        self.inputs = []
        self.outputs = []
        self.rel_base_offset = 0

    def send(self, k):
        self.inputs.append(k)

    def take(self):
        return self.outputs.pop(0)

    def get_address(self, i, mode):
        #print(i, mode)
        if mode == 0:
            return self.a.get(i, 0)
        elif mode == 1:
            return i
        else:
            return self.rel_base_offset + self.a.get(i, 0)


    def proc(self):

        a = self.a
        i = self.ip
        my_id = self.id
        r = None
        
        while True:

            op, mode3, mode2, mode1 = decode(a[i])

            if op == 99:
                return HALT        
            op1 = self.a.get(self.get_address(i + 1, mode1), 0)
            
            if op == 3:
                if len(self.inputs) > 0:
                    r = self.inputs.pop(0)
                else:
                    return AWAIT
                if DEBUG: print("{} received: {}".format(my_id, r))
                write_addr = self.get_address(i + 1, mode1)
                self.a[write_addr] = r
                i += 2
                self.ip = i
                continue
            elif op == 4:
                if DEBUG: print("{} generated {}".format(my_id, op1))
                self.outputs.append(op1)
                i += 2
                self.ip = i
                return None
            elif op == 9:
                self.rel_base_offset += op1
                i += 2
                self.ip = i
                continue
                

            op2 = self.a.get(self.get_address(i + 2, mode2), 0)

            if op == 5:
                if op1:
                    i = op2
                else:
                    i += 3
                self.ip = i
                continue

            if op == 6:
                if not op1:
                    i = op2
                else:
                    i += 3
                self.ip = i
                continue
          
            if op == 1:
                r = op1 + op2
            elif op  == 2:
                r = op1 * op2
            elif op == 7:
                r = int(op1 < op2)
            elif op == 8:
                r = int(op1 == op2)

            write_addr = self.get_address(i + 3, mode3)
            self.a[write_addr] = r

            #if mode3 == 0:
            #    a[a[i + 3]] = r
            #else:
            #    print("???")
            i += 4
            self.ip = i
            
        return (0, )        

DIR = [ (0, -1), (1, 0), (0, 1), (-1, 0) ]


def can_go(col, row, d, w, h, board):
    ncol = col + DIR[d][0]
    nrow = row + DIR[d][1]
    if 0 <= ncol < w and 0 <= nrow < h:
        if board[nrow][ncol] != '.':
            return True, ncol, nrow    
    
    return False, col, row
    
def trace(start, board):
    d = 0
    h = len(board)
    w = len(board[0])
    col, row = start
    
    result = []
    
    while True:
        print("test", col, row, d, w, h)
        ok, col, row = can_go(col, row, d, w, h, board)
        print(ok, col, row, d)
        
        if ok:
            if len(result) == 0 or type(result[-1]) != int:
                result.append(0)
            result[-1] +=1
        else:
            for p in [ ((d + 1) % 4, 'R'), ((d + 3) % 4, 'L') ]:
                d, ch = p
                ok, col, row = can_go(col, row, d, w, h, board)    
                print(ok, col, row, d)
                if ok:
                    result.append(ch)
                    result.append(1)
                    break

        if not ok:
            break
        
    #print(result)
    return result

def draw(board):
    for lst in board:
        print("".join(lst))

def part1():
    s =  open("input17.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    
    c = Proc(lst[:], 0)

    board = []

    curr = []
    res = c.proc()
    while res != HALT:
        code = c.outputs.pop(0)
        if code== 10 and len(curr) > 0:
            board.append(curr)
            curr = []
        else:
            curr.append(chr(code))
        res = c.proc()
            
    draw(board)
        
    h = len(board)
    w = len(board[0])
    
    result = 0
    
    start = None
    inter = []
    
    for row in range(1, h - 1):
        for col in range(1, w - 1):
            if board[row][col] not in ["#", '.']:
                start = (col, row)
            elif board[row][col] != '.':
                for d in DIR:
                    if board[row + d[1]][col + d[0]] == '.':
                        break
                else:
                    #print(col, row)
                    result += col * row
                    inter.append((col, row))

    print(result)
    print(start)
    print(inter)
    
    result = trace(start, board)
    for item in result:
        print(item, end=',')
    
    """
      
    R,8,L,12,R,8,R,8,L,12,R,8,L,10,L,10,R,8,L,12,L,12,L,10,R,10,L,10,L,10,R,8,L,12,L,12,L,10,R,10,L,10,L,10,R,8,R,8,L,12,R,8,L,12,L,12,L,10,R,10,R,8,L,12,R,8
    
    R,8,L,12,R,8,
    R,8,L,12,R,8,
    L,10,L,10,R,8,
    L,12,L,12,L,10,R,10,
    L,10,L,10,R,8,
    L,12,L,12,L,10,R,10,
    L,10,L,10,R,8,
    R,8,L,12,R,8,
    L,12,L,12,L,10,R,10,
    R,8,L,12,R,8
    
    A=R,8,L,12,R,8
    B=L,10,L,10,R,8
    C=L,12,L,12,L,10,R,10
    
    A,A,B,C,B,C,B,A,C,A
    
    
    """

def part2():

    s =  open("input17.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    lst[0] = 2
    
    c = Proc(lst[:], 0)

    s = "A,A,B,C,B,C,B,A,C,A\x0AR,8,L,12,R,8\x0AL,10,L,10,R,8\x0AL,12,L,12,L,10,R,10\x0An\x0A"
    si = 0

    while True:
        res = c.proc()
        if res == HALT:
            break
        elif res == AWAIT:
            c.send(ord(s[si]))        
            si += 1
        elif res is None:
            while len(c.outputs) > 0:
                code = c.outputs.pop(0)
                if (code <256) :
                    print(chr(code), end="")
                else:
                    print("Result:", code)
                    


    

def main():
    
    part1()

    part2()

if __name__ == "__main__":
    main()
