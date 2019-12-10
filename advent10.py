

STR = None

def get_arr():
    global STR
    s = """.#..#
.....
#####
....#
...##"""

    if STR is None:
        STR = open("input10.txt").read().strip()
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
    for y in range(len(a)):
        for x in range(len(a[0])):
            if a[y][x] == "#":
                b = get_arr()
                #print(b)
                vis = count_visible(b, x, y)
                max_vis = max(vis, max_vis)

    print("Part 1:",max_vis)

def main():
    part1()

if __name__ == "__main__":
    main()

    
