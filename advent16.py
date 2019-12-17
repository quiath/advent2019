    
D = False

AS = """59708072843556858145230522180223745694544745622336045476506437914986923372260274801316091345126141549522285839402701823884690004497674132615520871839943084040979940198142892825326110513041581064388583488930891380942485307732666485384523705852790683809812073738758055115293090635233887206040961042759996972844810891420692117353333665907710709020698487019805669782598004799421226356372885464480818196786256472944761036204897548977647880837284232444863230958576095091824226426501119748518640709592225529707891969295441026284304137606735506294604060549102824977720776272463738349154440565501914642111802044575388635071779775767726626682303495430936326809""" 

A = [ int(x) for x in AS ] 


# k is 1-based

def gen(ain, aout):
    p = [ 0 , 1, 0, -1 ] 
    n = len(ain)
   
    for k in range(1, n + 1):
        s = 0
        for i in range(n):
            m = p[((i + 1) // k) % 4]
            if D: print("{}*{}".format(ain[i],m), end=' ')
            s += ain[i] * m
        if D: print()
        aout[k - 1] = abs(s) % 10

    if D: print(aout)

# 0 based version without a shift in the pattern
# not useful after all

def gen0(ain, aout):
    p = [ 0 , 1, 0, -1 ] 
    n = len(ain)

    for k in range(1, n):
        s = 0
        for i in range(n):
            m = p[(i // k) % 4]
            if D: print("{}*{}".format(ain[i],m), end=' ')
            s += ain[i] * m
        if D: print()
        aout[k] = abs(s) % 10

    if D: print(aout)
    


def part1():
    a = A
    print(len(a))
    #a = [ int(x) for x in  "1 2 3 4 5 6 7 8".split() ] 
    b = [ 0 ] * len(a)

    for i in range(100 // 2):
        gen(a, b)
        gen(b, a)

    print(a)
    print("Part 1 solution:", "".join(str(x) for x in a[:8]))

def vis(n):
    p = [ 0 , 1, 0, -1 ] 

    for k in range(1, n + 1):
        for i in range(n):
            print("{:3}".format(p[i // k % 4]), end="")
        print()

def fast_compute_last_half(a):
    n = len(a)
    n2 = n // 2

    carried = 0
    i = n - 1
    while i >= n2:
        carried = carried + a[i]
        a[i] = abs(carried) % 10
        i -= 1


def main():

    part1()

    print("Helpful visualization of the coefficients for 21 (0-based)")
    vis(21)

    index = int(AS[:7])
    a_work = A[:] * 10000
    print("Position of the desired code in the long string (should be second half)", index, index / len(a_work))

    for i in range(100):
        fast_compute_last_half(a_work)

    for i in range(index, index + 8):
        print(a_work[i], end="")

    print(" is the code for part 2")

if __name__ == "__main__":
    main()
