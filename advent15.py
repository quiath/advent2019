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


DIR = { 1: (0, -1), 2: (0, 1), 3: (-1, 0), 4: (1, 0) }
#RDIR = { 2: (0, -1), 1: (0, 1), 4: (-1, 0), 3: (1, 0) }
REV = { 1: 2, 2: 1, 3: 4, 4: 3 }


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
    
    
def part1():
    s =  open("input15.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    
    c = Proc(lst[:], 0)

    board = {}
    x, y = 0, 0
    board[(x,y)] = 1

       
    d = 1

    q = [ [1 ], [2], [3], [4] ]

    while len(q) > 0:
        
        # forward
        path = q.pop(0)
        print("path", path)
        i = 0
        while i < len(path):
            res, x, y, tx, ty = step(c, path[i], x, y)
            board[(tx, ty)] = res 
            if res == 0:
                break
            if res == 2:
                draw(board)
                print("***\n", path, "\nresult=", len(path))
                return
            i += 1
        else:
            # end of path reached
            for k in DIR:
                xn, yn = x + DIR[k][0], y + DIR[k][1]
                if (xn, yn) not in board:
                    q.append(path + [ k ])

        #draw(board)
        # back
        j = i - 1 
        while j >= 0:
            res, x, y, tx, ty = step(c, REV[path[j]], x, y)
            j -= 1

        print("after path", path, x, y)


    
def draw(board):
    xx, yy = zip(*board.keys())
    minx = min(xx)
    maxx = max(xx)

    miny = min(yy)
    maxy = max(yy)

    for y in range(miny, maxy + 1):
        for x in range(minx, maxx + 1):
            if (x, y) in board:
                v = board[(x, y)]
                print("#.*"[v], end="")
            else:
                print(" ", end="")
        print()


def part2():
    s =  open("input15.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    lst[0] = 2

    board = {}

    c = Proc(lst[:], 0)

    bx = 0
    by = 0
    px = 0
    py = 0

    BALL = 4
    PADDLE = 3

    while True:
        #c.send(board[(x, y)])
        res = c.proc()
        if res == HALT:
            break;
        res = c.proc()
        if res == HALT:
            break;
        res = c.proc()
        if res == HALT:
            break;
            
        x = c.outputs.pop(0)
        y = c.outputs.pop(0)
        tile = c.outputs.pop(0)

        if x == -1:
            print("Score", tile)
            continue
        
                    
        board[(x, y)] = tile
        if tile == BALL:
            bx = x
            by = y
        elif tile == PADDLE:
            px = x
            py = y
 
        if tile == BALL:
            draw(board)
            if bx > px:
                c.send(1)
            elif bx < px:
                c.send(-1)
            else:
                c.send(0)





def main():
    
    part1()

    #draw(board)

    #part2()

    

if __name__ == "__main__":
    main()

    
