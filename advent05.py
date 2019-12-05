
def decode(x):
    s = "{:05}".format(x)
    opcode = int(s[3:])
    params = tuple(int(y) for y in s[:3])
    print("===", s, params)
    return (opcode,) + params

F = " +*}{Bb<="

def proc2(a, test_input = 1):
    
    i = 0
    while True:
        #print(a[:20])
        #print([(k, a[k]) for k in [6, 43, 224, 226]])

        op, mode3, mode2, mode1 = decode(a[i])

        #print("mode2=", mode2) 

        if op == 99:
            print(99)
            return
        
        """ 
        if 3 <= op <= 4:
            print("{} {}{}".format(F[op], "&#"[mode1], a[i + 1]))
        else:
            print("{} {}{} {}{} {}{}".format(F[op], "&#"[mode1], a[i + 1], "&#"[mode2], a[i + 2], "&#"[mode3], a[i + 3]))
        """

        op1 = a[a[i + 1]] if mode1 == 0 else a[i + 1]
        
        if op == 3:
            print(">")
            r = test_input # int(input())
            a[a[i + 1]] = r
            i += 2
            continue
        elif op == 4:
            print(">>>", op1)
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
        
    return a[0]        

"""
            continue

        op2 = a[a[i + 2]] if mode2 == 0 else a[i + 2]
        #print(type(op1), type(op2))

        if op == 1:
            r = op1 + op2
        elif op  == 2:
            r = op1 * op2
        if mode3 == 0:
            a[a[i + 3]] = r
        else:
            print("???")
        i += 4
        
    #print(a[0])
    return a[0]     
"""

def main():
    s =  open("input05.txt").read()
    #s = "1,9,10,3,2,3,11,0,99,30,40,50"
    #s = "1002,4,3,4,33"
    #s = "3,0,4,0,99"
    #s = "3,9,8,9,10,9,4,9,99,-1,8"
    lst = [ int(x) for x in s.split(",") ]
    
    # part 1
    proc2(lst, test_input = 1)
    # part 2
    proc2(lst, test_input = 1)



if __name__ == "__main__":
    main()
