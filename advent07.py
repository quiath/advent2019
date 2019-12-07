from itertools import permutations

DEBUG = False

def decode(x):
    s = "{:05}".format(x)
    opcode = int(s[3:])
    params = tuple(int(y) for y in s[:3])
    #print("===", s, params)
    return (opcode,) + params

F = " +*}{Bb<="

def proc(a, inputs = [3, 0] ):
    
    ret = []
    
    i = 0
    while True:
        #print(a[:20])
        #print([(k, a[k]) for k in [6, 43, 224, 226]])

        op, mode3, mode2, mode1 = decode(a[i])

        #print("mode2=", mode2) 

        if op == 99:
            print(99)
            return ret
        
        """ 
        if 3 <= op <= 4:
            print("{} {}{}".format(F[op], "&#"[mode1], a[i + 1]))
        else:
            print("{} {}{} {}{} {}{}".format(F[op], "&#"[mode1], a[i + 1], "&#"[mode2], a[i + 2], "&#"[mode3], a[i + 3]))
        """

        op1 = a[a[i + 1]] if mode1 == 0 else a[i + 1]
        
        if op == 3:
            print(">")
            r = inputs.pop(0)
            a[a[i + 1]] = r
            i += 2
            continue
        elif op == 4:
            print(">>>", op1)
            ret.append(op1)
            i += 2
            continue

        op2 = a[a[i + 2]] if mode2 == 0 else a[i + 2]
        #print(type(op1), type(op2))

        if op == 5:
            if op1:
                i = op2
            else:
                i += 3
            continue

        if op == 6:
            if not op1:
                i = op2
            else:
                i += 3
            continue

        

        if op == 1:
            r = op1 + op2
        elif op  == 2:
            r = op1 * op2
        elif op == 7:
            r = int(op1 < op2)
        elif op == 8:
            r = int(op1 == op2)


        if mode3 == 0:
            a[a[i + 3]] = r
        else:
            print("???")
        i += 4
        
    return ret        


def part1():
    s =  open("input07.txt").read()
    #s = "3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0"

    #s = "3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0"
    lst = [ int(x) for x in s.split(",") ]
    
    # part 1

    max_signal = 0
    for perm in permutations([0, 1, 2, 3, 4]):
        print(perm)

        a = lst[:]
        
        signal = 0

        for phase in perm:
            print("#", phase, signal)
            inputs = [ phase, signal ]
            signal = proc(a, inputs)[0]
        
        print("##", signal, max_signal)
        max_signal = max(signal, max_signal)
    print("### max signal part 1 ", max_signal)


class Proc:
    def __init__(self, a, my_id):
        self.a = a
        self.id = my_id
        self.ip = 0
        self.inputs = []
        self.outputs = []

    def send(self, k):
        self.inputs.append(k)

    def take(self):
        return self.outputs.pop(0)

    def proc(self):

        a = self.a
        i = self.ip
        my_id = self.id
        
        while True:

            op, mode3, mode2, mode1 = decode(a[i])

            if op == 99:
                return (99,)        
            op1 = a[a[i + 1]] if mode1 == 0 else a[i + 1]
            
            if op == 3:
                r = self.inputs.pop(0)
                if DEBUG: print("{} received: {}".format(my_id, r))
                a[a[i + 1]] = r
                i += 2
                self.ip = i
                continue
            elif op == 4:
                if DEBUG: print("{} generated {}".format(my_id, op1))
                self.outputs.append(op1)
                i += 2
                self.ip = i
                return None

            op2 = a[a[i + 2]] if mode2 == 0 else a[i + 2]

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

            if mode3 == 0:
                a[a[i + 3]] = r
            else:
                print("???")
            i += 4
            self.ip = i
            
        return (0, )        


def perm_result(lst, perm):
    c = [ Proc(lst[:], i) for i in range(5) ]
    for i in range(5):
        c[i].send(perm[i])
        
    signal = 0
    while True:
        for i in range(5):
            if DEBUG: print("Sending", signal)
            c[i].send(signal)
            ret = c[i].proc() 
            
            if ret == (99,):
                if DEBUG: print("Final is ", signal)
                return signal

            out = c[i].take()
            if DEBUG: print("c[{}] returned {} {}".format(i, ret, out))
            signal = out 
    

def part2():
    s = "3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5"

    s = ( "3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54," +
          "-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4," +
          "53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10")
    s =  open("input07.txt").read()


    lst = [ int(x) for x in s.split(",") ]

    max_signal = 0
    for perm in permutations([5, 6, 7, 8, 9]):
        print(perm)
        signal = perm_result(lst, perm)
        max_signal = max(max_signal, signal)

    print("Max for part 2 is ", max_signal)



def main():
    
    part1()
    part2()

if __name__ == "__main__":
    main()

    
