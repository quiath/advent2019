from collections import defaultdict

DEBUG = False

HALT = (99,)
AWAIT = (3,)
STEP = (0,)

def decode(x):
    s = "{:05}".format(x)
    #s = "{:05}".format(abs(x))
    #print(s)
    opcode = int(s[3:])
    params = tuple(int(y) for y in s[:3])
    #print("===", s, params)
    return (opcode,) + params



class Proc:
    def __init__(self, a, my_id, await_input = None, step_mode = False):
        self.a = dict((i, a[i]) for i in range(len(a)))
        self.id = my_id
        self.ip = 0
        self.inputs = []
        self.outputs = []
        self.rel_base_offset = 0
        self.await_input = await_input
        self.step_mode = step_mode

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

        steps = 0
        
        while True:
            steps += 1
            if self.step_mode and steps >= 2:
                return STEP

            op, mode3, mode2, mode1 = decode(a[i])

            if op == 99:
                return HALT        
            op1 = self.a.get(self.get_address(i + 1, mode1), 0)
            
            if op == 3:
                if len(self.inputs) > 0:
                    r = self.inputs.pop(0)
                else:
                    if self.await_input is None:
                        return AWAIT
                    else:
                        r = self.await_input
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
            
        return STEP       
    
 
def part1():

    prgs =  open("input23.txt").read()

    lst = [ int(x) for x in prgs.split(",") ]

    N = 50

    comp = []
    for i in range(N):
        #c = Proc(lst[:], i, await_input = -1)
        c = Proc(lst[:], i, await_input = None)
        c.send(i)
        comp.append(c)

    outputs = defaultdict(list)
    index = defaultdict(int)

    i = -1
    while True:

        i += 1
        if i == N:
            i = 0
        
        c = comp[i]
        res = c.proc()
        print(i, res)
        if res == HALT:
            break
        elif res == AWAIT:
            #c.send(ord(s[si]))        
            #si += 1
            print(i, "AWAIT")
            c.send(-1)
        elif res is None:
            while len(c.outputs) > 0:
                code = c.outputs.pop(0)
                outputs[i].append(code)
                print("Proc ", i, "generated", code)
            
            ind = index[i]
            if ind + 3 <= len(outputs[i]):
                index[i] = ind + 3
            
                send_id = outputs[i][ind] 
                if send_id >= N:
                    print(outputs[i][ind:])
                    if send_id == 255:
                        break
                else:
                    comp[send_id].send(outputs[i][ind + 1])
                    comp[send_id].send(outputs[i][ind + 2])
        elif res == STEP:
            print(i, "Step") 




def main():
    
    part1()

    #part2()


if __name__ == "__main__":
    main()
