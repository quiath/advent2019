
DIR = { 'U': (0, -1), 'L': (-1, 0), 'D': (0, 1), 'R': (1, 0) }


def decode(s):
    return (DIR[s[0]], int(s[1:]))

def part1():
    data = [ "R75,D30,R83,U83,L12,D49,R71,U7,L72",
              "U62,R66,U55,R34,D71,R55,D58,R83" ] 

    #data = [ "R8,U5,L5,D3", "U7,R6,D4,L4" ]

    data = [ x.strip() for x in open("input03.txt").readlines() ]

    mindist = 10**10
    acc = set()

    for line in data:
        elems = [ decode(x) for x in line.split(",") ]
        print(elems)

        curr = set()
        
        x, y  = 0, 0

        for e in elems:
            d, n = e
            dx, dy = d

            for i in range(n):
                x += dx
                y += dy

                curr.add((x, y))
                #print((x, y))

        inter = acc & curr

        for e in inter:
            dist = abs(e[0]) + abs(e[1])
            mindist = min(mindist, dist)

        acc.update(curr)


    print(mindist)


def part2():
    data = [ "R75,D30,R83,U83,L12,D49,R71,U7,L72",
              "U62,R66,U55,R34,D71,R55,D58,R83" ] 

    #data = [ "R8,U5,L5,D3", "U7,R6,D4,L4" ]

    data = [ x.strip() for x in open("input03.txt").readlines() ]

    mindist = 10**10
    acc = set()

    dmap = {}

    for line in data:
        elems = [ decode(x) for x in line.split(",") ]
        print(elems)

        curr = set()
        cdmap = {}

        x, y  = 0, 0
        step = 0

        for e in elems:
            d, n = e
            dx, dy = d

            for i in range(n):
                x += dx
                y += dy
                step += 1

                curr.add((x, y))
                if (x, y) not in cdmap:
                    cdmap[(x, y)] = step
                #print((x, y, step))

        inter = acc & curr
        print(inter)
        for pos in inter:
            
            dist = dmap[pos] + cdmap[pos]
            mindist = min(mindist, dist)

        acc.update(curr)
        dmap = cdmap.copy()
        
    print(mindist)

    

def main():
    part2()   



if __name__ == "__main__":
    main()
