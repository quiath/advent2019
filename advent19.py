from collections import defaultdict

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


def step(c, inp, x, y):
    c.send(inp)
    res = c.proc()
    if res == HALT:
        return HALT, x, y, x, y
                
    res = c.outputs.pop(0)

    tx, ty = x + DIR[inp][0], y + DIR[inp][1]

    if res > 0:
        x = tx 
        y = ty

    return res, x, y, tx, ty




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

def ask(x, y, lst):
    c = Proc(lst[:], 0)
    c.send(x)        
    c.send(y)        
    
    res = c.proc()
    #print(res)
    return c.outputs.pop(0)

def part1():
    s =  open("input19.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    
    
    
    ret = []
    
    for y in range(50):
        left, right = 100, -1
        for x in range(50):    
            c = Proc(lst[:], 0)
            c.send(x)        
            c.send(y)        
            
            res = c.proc()
            #print(res)
            tractor = c.outputs.pop(0)
            ret.append(tractor)
            
            if y < 20 and x < 20:
                if tractor:
                    left = min(left, x)
                    right = max(right, x)
                print(".#"[tractor], end="")
                
        if y < 20:
            print(" {}-{},{}".format(left, right, y))
            """
            if res == HALT:
                break
            elif res is None:
                ret.append(c.outputs.pop(0))
            """
    #print(ret)
    print(ret.count(1))
    
def part2():

    s =  open("input19.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    
    # manually seeded    
    left = { 9: 10}
    right = { 9: 12}
    
    y = 10
    
    while True:
        
        x = left[y - 1] + 1
        r = ask(x, y, lst)
        
        if r:
            left[y] = x
            x -= 1
            while x >= 0:
                r = ask(x, y, lst)
                if r:
                    left[y] = x
                else:
                    break
                x -= 1
        else:
            x += 1
            while True:
                r = ask(x, y, lst)
                if r:
                    left[y] = x
                    break
                x += 1

        x = right[y - 1]
        
        r = ask(x, y, lst)
        if r:
            
            while True:
                r = ask(x, y, lst)
                if r:
                    right[y] = x
                else:
                    break
                x += 1


        #print(left[y], right[y])
        
        
        extent = 100 - 1 # had an off by one error here
        
        x0 = left[y]
        y0 = y - extent 
        
        if x0 > 11 and y0 > 11:
            if left[y0] <= x0 and (x0 + extent) <= right[y0]:
                
                print("Found")
                print(x0, y0)
                print(left[y0], right[y0])
                print(left[y], right[y])
                print(x0 * 10000 + y0)
                break

        y += 1
        if y > 1000000:
            break
    

def main():
    part1()
    part2()

if __name__ == "__main__":
    main()
