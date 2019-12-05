

def proc(a):
    step = 4
    i = 0
    while (a[i] != 99):
        op1 = a[i + 1]
        op2 = a[i + 2]
        if a[i] == 1:
            r = a[op1] + a[op2]
        elif a[i] == 2:
            r = a[op1] * a[op2]
        a[a[i + 3]] = r
        i += step

    #print(a[0])
    return a[0]        

def main():
    s =  open("input02.txt").read()
    #s = "1,9,10,3,2,3,11,0,99,30,40,50"
    lst = [ int(x) for x in s.split(",") ]
    lst[1] = 12
    lst[2] = 2
    print(proc(lst))

    for noun in range(100):
        for verb in range(100):
            lst = [ int(x) for x in s.split(",") ]
            lst[1] = noun
            lst[2] = verb
            r = proc(lst)
            if r == 19690720:
                print(noun, verb, 100 * noun + verb)
                return


if __name__ == "__main__":
    main()
