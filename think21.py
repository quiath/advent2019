
def test():
  
    s = "#####.##.#############.#.##.##.###############.####.###########...#############.#..###############.####...###############..###########.#.#.#.#########..#.################.####...####" + ("#" * 10)
    for i in range(9):
        j = 0
        print(s)
        while j + 9 < len(s):

            if s[j] == ".":
                print(j)
                print("{}@{}".format(s[:j], s[j+1:]))
                return False

            a = s[j + 1] == "#"
            b = s[j + 2] == "#"
            c = s[j + 3] == "#"
            d = s[j + 4] == "#"
            e = s[j + 5] == "#"
            f = s[j + 6] == "#"
            g = s[j + 7] == "#"
            h = s[j + 8] == "#"
            i = s[j + 9] == "#"

            r = not (a and b and c) and d and (e or h) #ok

            if r:
                j += 4
            else:
                j += 1

        else:
            print("ok")
        
        s = "#" + s
    return True
    

def main():
    ret = test()
    print(ret)
   


if __name__ == "__main__":
    main()
