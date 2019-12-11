from collections import defaultdict

DEBUG = False

HALT = (99,)

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
        
        while True:

            op, mode3, mode2, mode1 = decode(a[i])

            if op == 99:
                return HALT        
            op1 = self.a.get(self.get_address(i + 1, mode1), 0)
            
            if op == 3:
                r = self.inputs.pop(0)
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




def part1(lst):
    c = Proc(lst[:], 0)
    c.send(1)

    res = None
    while res == None:
        res = c.proc()
    print(c.outputs)
    

def main():
    
    s =  open("input11.txt").read()
    
    board = defaultdict(int)

    lst = [ int(x) for x in s.split(",") ]
    
    D = [(0, -1), (1, 0), (0, 1), (-1, 0)]
    x, y = 0, 0
    d = 0
    c = Proc(lst[:], 0)

    while True:
        c.send(board[(x, y)])
        res = c.proc()
        if res == HALT:
            break;
        res = c.proc()
        if res == HALT:
            break;

        color = c.outputs.pop(0)
        direction = c.outputs.pop(0)

        board[(x, y)] = color
        if direction == 0:
            d = (d + 4 - 1) % 4
        else:
            d = (d + 1) % 4

        x += D[d][0]
        y += D[d][1]

    print(len(board))

if __name__ == "__main__":
    main()

    
