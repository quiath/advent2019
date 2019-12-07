from itertools import permutations

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
    #proc(lst, input = 1)

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
    print("### max signal ", max_signal)


def main():
    part1()

if __name__ == "__main__":
    main()
