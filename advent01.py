
def total_m(m):
    t = 0
    m = (m // 3) - 2 
    while m > 0:
        t += m
        m = (m // 3) - 2 
    return t

def main():
    lst = [ int(x) for x in open("input01.txt").read().split() ]
    msum = sum([ (m // 3) - 2 for m in lst])
    print(msum)

    msum2 = sum([ total_m(m) for m in lst])

    print(msum2)

if __name__ == "__main__":
    main()
