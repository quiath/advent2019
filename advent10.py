from fractions import Fraction as F

STR = None
TESTCASE = 0


def get_arr():
    global STR

    if STR is None:
        if TESTCASE == 0:
            STR = open("input10.txt").read().strip()
        elif TESTCASE == 1:
            STR = """.#....#####...#..
##...##.#####..##
##...#...#.#####.
..#.....X...###..
..#.#.....#....##"""
        elif TESTCASE == 2:
            STR = """#######
#######
#######
#######
#######
#######
#######"""
        elif TESTCASE == 3:
            STR = """.#..##.###...#######
##.############..##.
.#.######.########.#
.###.#######.####.#.
#####.##.#.##.###.##
..#####..#.#########
####################
#.####....###.#.#.##
##.#################
#####.##.###..####..
..######..##.#######
####.##.####...##..#
.#####..#.######.###
##...#.##########...
#.##########.#######
.####.#.###.###.#.##
....##.##.###..#####
.#.#.###########.###
#.#.#.#####.####.###
###.##.####.##.#..##"""

        elif TESTCASE == 4:
            STR = """.#..#
.....
#####
....#
...##"""
    s = STR

    a = []
    for line in s.split('\n'):
        a.append([x for x in line.strip()])

    return a

def draw_ray(a, x0, y0, dx, dy, w, h):
    aster = False
    x = x0 + dx
    y = y0 + dy
    while (0 <= x < w) and (0 <= y < h):
        if a[y][x] == "#" and not aster:
            aster = True
        a[y][x] = " "
        x += dx
        y += dy

    return aster


def count_visible(a, x0, y0):
    h = len(a)
    w = len(a[0])

    rmax = max([x0, y0, w - x0 - 1, h - y0 - 1])

    count = 0

    for r in range(1, rmax + 1):
        # horizontal
        for x in range(max(x0 - r, 0), min(x0 + r + 1, w)):
            for sgn in [-1, 1]:
                y = y0 + sgn * r
                if 0 <= y < h:
                    if a[y][x] != " ":
                        c = draw_ray(a, x0, y0, x - x0, y - y0, w, h)
                        count += c
        # vertical
        for y in range(max(y0 - r, 0), min(y0 + r + 1, h)):
            #print(max(y0 - r, 0), min(y0 + r + 1, h + 1))
            for sgn in [-1, 1]:
                x = x0 + sgn * r
                if 0 <= x < w:
                    if a[y][x] != " ":
                        c = draw_ray(a, x0, y0, x - x0, y - y0, w, h)
                        count += c

    return count


                 
                
def part1():
    a = get_arr()
    print(a)
    #print(count_visible(a, 3, 4))
    #print(a)

    max_vis = 0
    pos = None
    for y in range(len(a)):
        for x in range(len(a[0])):
            if a[y][x] == "#":
                b = get_arr()
                #print(b)
                vis = count_visible(b, x, y)
                if vis > max_vis:
                    max_vis = vis
                    pos = (x, y)

    print("Part 1:",max_vis, pos)
    return pos

def laser(a, lst, goal = 200):

    cnt = 0
    while True:
        prev = None
        i = 0
        if len(lst) == 0:
            return

        while i < len(lst):
            e = lst[i]

            if prev is None or len(lst) == 1 or prev[0] != e[0] or prev[1] != e[1]:
                prev = (e[0], e[1])
                x, y = e[4], e[5]
                if cnt % 9 == 0:
                    for yy in range(len(a)):
                        for xx in range(len(a[0])):
                            if '0' <= a[yy][xx] <= '9':
                                a[yy][xx] = "."
                a[y][x] = chr(48 + cnt % 9 + 1)
                cnt += 1
                print(cnt, e)
                if cnt == goal:
                    print("The asteroid:", goal, "info is:", e)
                    return
                del lst[i]
            else:
                i += 1
            if True:
                for line in a:
                    print("".join(line))
                print()


def main():

    positions = { 1: (8, 3), 2 : (3, 3), 3 : (11, 13) }
    if TESTCASE in positions:
        pos = positions[TESTCASE] 

    else:
        pos = part1()
    
    # part 2

    a = get_arr()
    h = len(a)
    w = len(a[0])

    for line in a:
        print(line)
    
    print()
    for line in a:
        print("".join(line))

    # divide the board into 8 octants (8 triangles)
    # create a list of asteroids starting with the octants 

    # sort asteroids in each octant by the ratio of x offset to y offset (negated or reciprocal as appropriate) 
    # carefully include points on the vertical, horizontal and diagonal lines - only once! 

    # the tag is used to keep separate asteroids with the same ratio but belonging to other octants 
    # actually with the tag you could sort only once

    octo1 = sorted([ (1, F(dx, dy), dy, dx, pos[0] + dx, pos[1] - dy) for dx in range(0, w) for dy in range(1, h)
               if (dy > dx) and (0 <= pos[0] + dx < w) and (0 <= pos[1] - dy < h) and a[pos[1] - dy][pos[0] + dx] == "#" ])

    octo2 = sorted([ (2, -F(dy, dx), dx, dy, pos[0] + dx, pos[1] - dy) for dx in range(1, w) for dy in range(1, h)
               if (dx >= dy) and (0 <= pos[0] + dx < w) and (0 <= pos[1] - dy < h) and a[pos[1] - dy][pos[0] + dx] == "#" ])

    octo3 = sorted([ (3, F(dy, dx), dx, dy, pos[0] + dx, pos[1] + dy) for dx in range(1, w) for dy in range(0, h)
               if (dx > dy) and (0 <= pos[0] + dx < w) and (0 <= pos[1] + dy < h) and a[pos[1] + dy][pos[0] + dx] == "#" ])

    octo4 = sorted([ (4, -F(dx, dy), dy, dx, pos[0] + dx, pos[1] + dy) for dx in range(1, w) for dy in range(1, h)
               if (dy >= dx) and (0 <= pos[0] + dx < w) and (0 <= pos[1] + dy < h) and a[pos[1] + dy][pos[0] + dx] == "#" ])

    octo5 = sorted([ (5, F(dx, dy), dy, dx, pos[0] - dx, pos[1] + dy) for dx in range(0, w) for dy in range(1, h)
               if (dy > dx) and (0 <= pos[0] + dx < w) and (0 <= pos[1] + dy < h) and a[pos[1] + dy][pos[0] - dx] == "#" ])

    octo6 = sorted([ (6, -F(dy, dx), dx, dy, pos[0] - dx, pos[1] + dy) for dx in range(1, w) for dy in range(1, h)
               if (dx >= dy) and (0 <= pos[0] - dx < w) and (0 <= pos[1] + dy < h) and a[pos[1] + dy][pos[0] - dx] == "#" ])

    octo7 = sorted([ (7, F(dy, dx), dx, dy, pos[0] - dx, pos[1] - dy) for dx in range(1, w) for dy in range(0, h)
               if (dx > dy) and (0 <= pos[0] - dx < w) and (0 <= pos[1] - dy < h) and a[pos[1] - dy][pos[0] - dx] == "#" ])

    octo8 = sorted([ (8, -F(dx, dy), dy, dx, pos[0] - dx, pos[1] - dy) for dx in range(1, w) for dy in range(1, h)
               if (dy >= dx) and (0 <= pos[0] - dx < w) and (0 <= pos[1] - dy < h) and a[pos[1] - dy][pos[0] - dx] == "#" ])



    a[pos[1]][pos[0]] = 'o'


    laser(a, octo1 + octo2 + octo3 + octo4 + octo5 + octo6 + octo7 + octo8)


if __name__ == "__main__":
    main()

    
