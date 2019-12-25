from collections import defaultdict
import random
import sys
DEBUG = False

# TODO:
# actual real BFS or DFS routine instead of random walk
# pick up items while getting the layout
# recognize the infinite loops when picking bad items and automatically interrupt
# shorten the walk to the security checkpoint instead of going via the start point
# use the lighter / heavier information from the check instead of the brute force iteration
# but... still this worked and that's what counts!!!

HALT = (99,)
AWAIT = (3,)

def decode(x):
    s = "{:05}".format(x)
    opcode = int(s[3:])
    params = tuple(int(y) for y in s[:3])
    #print("===", s, params)
    return (opcode,) + params



class Proc:
    def __init__(self, a, my_id):
        self.a = dict((i, a[i]) for i in range(len(a)))
        self.id = my_id
        self.ip = 0
        self.inputs = []
        self.outputs = []
        self.rel_base_offset = 0

    def send(self, k):
        self.inputs.append(k)

    def take(self):
        return self.outputs.pop(0)

    def get_address(self, i, mode):
        #print(i, mode)
        if mode == 0:
            return self.a.get(i, 0)
        elif mode == 1:
            return i
        else:
            return self.rel_base_offset + self.a.get(i, 0)


    def proc(self):

        a = self.a
        i = self.ip
        my_id = self.id
        r = None
        
        while True:

            op, mode3, mode2, mode1 = decode(a[i])

            if op == 99:
                return HALT        
            op1 = self.a.get(self.get_address(i + 1, mode1), 0)
            
            if op == 3:
                if len(self.inputs) > 0:
                    r = self.inputs.pop(0)
                else:
                    return AWAIT
                if DEBUG: print("{} received: {}".format(my_id, r))
                write_addr = self.get_address(i + 1, mode1)
                self.a[write_addr] = r
                i += 2
                self.ip = i
                continue
            elif op == 4:
                if DEBUG: print("{} generated {}".format(my_id, op1))
                self.outputs.append(op1)
                i += 2
                self.ip = i
                return None
            elif op == 9:
                self.rel_base_offset += op1
                i += 2
                self.ip = i
                continue
                

            op2 = self.a.get(self.get_address(i + 2, mode2), 0)

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

            write_addr = self.get_address(i + 3, mode3)
            self.a[write_addr] = r

            #if mode3 == 0:
            #    a[a[i + 3]] = r
            #else:
            #    print("???")
            i += 4
            self.ip = i
            
        return (0, )        

DIR = [ (0, -1), (1, 0), (0, 1), (-1, 0) ]


def step(c, inp, x, y):
    c.send(inp)
    res = c.proc()
    if res == HALT:
        return HALT, x, y, x, y
                
    res = c.outputs.pop(0)

    tx, ty = x + DIR[inp][0], y + DIR[inp][1]

    if res > 0:
        x = tx 
        y = ty

    return res, x, y, tx, ty




def can_go(col, row, d, w, h, board):
    ncol = col + DIR[d][0]
    nrow = row + DIR[d][1]
    if 0 <= ncol < w and 0 <= nrow < h:
        if board[nrow][ncol] != '.':
            return True, ncol, nrow    
    
    return False, col, row
    
def trace(start, board):
    d = 0
    h = len(board)
    w = len(board[0])
    col, row = start
    
    result = []
    
    while True:
        print("test", col, row, d, w, h)
        ok, col, row = can_go(col, row, d, w, h, board)
        print(ok, col, row, d)
        
        if ok:
            if len(result) == 0 or type(result[-1]) != int:
                result.append(0)
            result[-1] +=1
        else:
            for p in [ ((d + 1) % 4, 'R'), ((d + 3) % 4, 'L') ]:
                d, ch = p
                ok, col, row = can_go(col, row, d, w, h, board)    
                print(ok, col, row, d)
                if ok:
                    result.append(ch)
                    result.append(1)
                    break

        if not ok:
            break
        
    #print(result)
    return result

def draw(board):
    for lst in board:
        print("".join(lst))

def ask(x, y, lst):
    c = Proc(lst[:], 0)
    c.send(x)        
    c.send(y)        
    
    res = c.proc()
    #print(res)
    return c.outputs.pop(0)

def part1():
    s =  open("input19.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    
    
    
    ret = []
    
    for y in range(50):
        left, right = 100, -1
        for x in range(50):    
            c = Proc(lst[:], 0)
            c.send(x)        
            c.send(y)        
            
            res = c.proc()
            #print(res)
            tractor = c.outputs.pop(0)
            ret.append(tractor)
            
            if y < 20 and x < 20:
                if tractor:
                    left = min(left, x)
                    right = max(right, x)
                print(".#"[tractor], end="")
                
        if y < 20:
            print(" {}-{},{}".format(left, right, y))
            """
            if res == HALT:
                break
            elif res is None:
                ret.append(c.outputs.pop(0))
            """
    #print(ret)
    print(ret.count(1))
    
def part2():

    s =  open("input19.txt").read()

    lst = [ int(x) for x in s.split(",") ]
    
    # manually seeded    
    left = { 9: 10}
    right = { 9: 12}
    
    y = 10
    
    while True:
        
        x = left[y - 1] + 1
        r = ask(x, y, lst)
        
        if r:
            left[y] = x
            x -= 1
            while x >= 0:
                r = ask(x, y, lst)
                if r:
                    left[y] = x
                else:
                    break
                x -= 1
        else:
            x += 1
            while True:
                r = ask(x, y, lst)
                if r:
                    left[y] = x
                    break
                x += 1

        x = right[y - 1]
        
        r = ask(x, y, lst)
        if r:
            
            while True:
                r = ask(x, y, lst)
                if r:
                    right[y] = x
                else:
                    break
                x += 1


        #print(left[y], right[y])
        
        
        extent = 100 - 1 # had an off by one error here
        
        x0 = left[y]
        y0 = y - extent 
        
        if x0 > 11 and y0 > 11:
            if left[y0] <= x0 and (x0 + extent) <= right[y0]:
                
                print("Found")
                print(x0, y0)
                print(left[y0], right[y0])
                print(left[y], right[y])
                print(x0 * 10000 + y0)
                break

        y += 1
        if y > 1000000:
            break
    

DIRS = { "north": -1, "south": 1, "east": 2, "west": -2, "": 10 }
RDIR = { -1: "north", 1: "south", 2: "east", -2: "west" }


class State:
    def __init__(self):
        self.location = ""
        self.prevlocation = "START"
        self.x = 0
        self.y = 0
        self.path = ""
        self.command = ""
        self.way = dict()
        self.pos = dict()
        self.pos["== Hull Breach =="] = (0, 0)
        self.curpath = []
        self.path = dict()
        self.door = defaultdict(set)
        self.item = defaultdict(set)

    def add_door(self, direction):
        self.door[self.location].add(direction)
    def add_item(self, item):
        self.item[self.location].add(item)

    def set_location(self, location):
        if self.location not in self.way:
           self.way[self.location] = dict()
        self.way[self.location][self.command] = location

        if location not in self.pos and self.command != "" and self.prevlocation in self.pos:
            x, y = self.pos[self.prevlocation]
            if self.command == "north":
                self.pos[location] = x, y - 1
            if self.command == "south":
                self.pos[location] = x, y + 1
            if self.command == "west":
                self.pos[location] = x - 1, y
            if self.command == "east":
                self.pos[location] = x + 1, y

        if location not in self.path:
            self.curpath.append(self.command)
            self.path[location] = self.curpath[:]
        else:
            print(self.curpath, "replaced by", self.path[location])
            self.curpath=self.path[location][:]
        print(self.path)
        print(self.curpath)
        #xxx=input()
        
        """
        if len(self.curpath) >= 1 and DIRS[self.curpath[-1]] == -DIRS[self.command]:
            self.curpath.pop()
        else:
            self.curpath.append(self.command)
        """
        print(location, self.pos.get(location, (0, 0)), self.curpath)

        self.prevlocation = self.location
        self.location = location

        self.command = ""
        #print(self.way)
        

def autorun():

    prgs =  open("input25.txt").read()

    log = open("log25.txt", "a")

    lst = [ int(x) for x in prgs.split(",") ]
    
    c = Proc(lst[:], 0)


    #s = "\x0A".join([x.strip() for x in springcode.strip().splitlines()]) + "\x0A"
    s = ""
    si = 0

    outline = ""
    state = State()

    stepcnt = 0 

    door_flag = False
    item_flag = False

    while True:
        stepcnt += 1
        if stepcnt >= 100000:
            break
        res = c.proc()
        if res == HALT:
            print("HALT")
            break
        elif res == AWAIT:
            if si >= len(s):
                s = random.choice(["north", "east", "south", "west"])
                state.command = s
                s = s + "\x0A"
                si = 0
                print(s, file=log)
            c.send(ord(s[si]))        
            si += 1
        elif res is None:
            while len(c.outputs) > 0:
                code = c.outputs.pop(0)
                if (code < 256) :
                    print(chr(code), end="")
                    print(chr(code), end="", file=log)
                    if code == 10:
                        print(">>{}<<".format(outline))
                        if outline.startswith("=="):
                            state.set_location(outline)
                            door_flag, item_flag = False, False
                        elif outline.startswith("Doors here lead:"):
                            door_flag, item_flag = True, False
                        elif outline.startswith("Items here:"):
                            door_flag, item_flag = False, True
                        else:
                            if door_flag or item_flag:
                                if outline.startswith("- "):
                                    if door_flag:
                                        state.add_door(outline[2:])
                                    if item_flag:
                                        state.add_item(outline[2:])
                                else:
                                   
                                    door_flag, item_flag = False, False
                    

                        outline = ""
                        
                    else:
                        outline = outline + chr(code)
                else:
                    print("Result:", code)
        """                    
        if res == HALT:
            break
        elif res == AWAIT:
            if si >= len(s):
                s = input()
                s = s + "\x0A"
                si = 0
                print(s, file=log)
            c.send(ord(s[si]))        
            si += 1
        elif res is None:
            while len(c.outputs) > 0:
                code = c.outputs.pop(0)
                if (code < 256) :
                    if code == 10:
                        if outline.startswith("=="):
                            state.set_location(location)
                    else:
                        outline = outline + chr(code)
                    print(chr(code), end="")
                    print(chr(code), end="", file=log)
                else:
                    print("Result:", code)
        """            

    for loc in state.way:
        d = state.way[loc]
        for cmd in d:
            print(loc, cmd, d[cmd])

    for loc in state.pos:
        print(loc, state.pos[loc])

    for loc in state.path:
        print('"{}": {},'.format(loc, state.path[loc]))


    for loc in state.item:
        print('"{}": {},'.format(loc, list(state.item[loc])))

    for loc in state.door:
        print('"{}": {},'.format(loc, list(state.door[loc])))

PATH = {
"== Hull Breach ==": [''],
"== Hallway ==": ['', 'west'],
"== Science Lab ==": ['', 'south'],
"== Holodeck ==": ['', 'west', 'west'],
"== Passages ==": ['', 'west', 'west', 'north'],
"== Navigation ==": ['', 'west', 'south'],
"== Arcade ==": ['', 'west', 'south', 'east'],
"== Storage ==": ['', 'west', 'south', 'south'],
"== Engineering ==": ['', 'east'],
"== Sick Bay ==": ['', 'east', 'east'],
"== Crew Quarters ==": ['', 'east', 'east', 'south'],
"== Warp Drive Maintenance ==": ['', 'east', 'east', 'south', 'west'],
"== Observatory ==": ['', 'east', 'east', 'south', 'west', 'south'],
"== Gift Wrapping Center ==": ['', 'east', 'east', 'south', 'west', 'south', 'west'],
"== Security Checkpoint ==": ['', 'east', 'east', 'south', 'west', 'south', 'west', 'south'],
"== Hot Chocolate Fountain ==": ['', 'east', 'east', 'south', 'east'],
"== Corridor ==": ['', 'east', 'east', 'south', 'east', 'south'],
"== Stables ==": ['', 'east', 'east', 'south', 'east', 'north'],
"== Pressure-Sensitive Floor ==": ['', 'east', 'east', 'south', 'west', 'south', 'west', 'south', 'south']
}
INITITEM = {
"== Science Lab ==": ['weather machine'],
"== Holodeck ==": ['giant electromagnet'],
"== Passages ==": ['space heater'],
"== Navigation ==": ['festive hat'],
"== Arcade ==": ['whirled peas'],
"== Storage ==": ['sand'],
"== Engineering ==": ['mug'],
"== Sick Bay ==": ['escape pod'],
"== Observatory ==": ['infinite loop'],
"== Gift Wrapping Center ==": ['shell'],
"== Hot Chocolate Fountain ==": ['photons'],
"== Corridor ==": ['easter egg'],
"== Stables ==": ['molten lava'],
}
DOORS = {
"== Hull Breach ==": ['east', 'west', 'south'],
"== Hallway ==": ['east', 'west', 'south'],
"== Science Lab ==": ['north'],
"== Holodeck ==": ['east', 'north'],
"== Passages ==": ['south'],
"== Navigation ==": ['east', 'south', 'north'],
"== Arcade ==": ['west'],
"== Storage ==": ['north'],
"== Engineering ==": ['east', 'west'],
"== Sick Bay ==": ['west', 'south'],
"== Crew Quarters ==": ['east', 'west', 'north'],
"== Warp Drive Maintenance ==": ['east', 'south'],
"== Observatory ==": ['west', 'north'],
"== Gift Wrapping Center ==": ['east', 'south'],
"== Security Checkpoint ==": ['south', 'north'],
"== Hot Chocolate Fountain ==": ['west', 'south', 'north'],
"== Corridor ==": ['north'],
"== Stables ==": ['west', 'south'],
"== Pressure-Sensitive Floor ==": ['north']
}


def gen_to(name):
    p = PATH[name]
    r = []
    for cmd in p:
        if cmd != '':
            r.append(cmd)
    return r


def gen_from(name):
    p = PATH[name]
    r = []
    for cmd in p:
        if cmd != '':
            k = DIRS[cmd]

            r.append(RDIR[-k])
    r.reverse()
    return r


def runsemi():

    prgs =  open("input25.txt").read()

    log = open("log25.txt", "a")

    lst = [ int(x) for x in prgs.split(",") ]
    
    c = Proc(lst[:], 0)

    commands = []

    good_items = []

    for room in INITITEM:
        for item in INITITEM[room]:
            if item in [ "giant electromagnet", "molten lava", "escape pod", "infinite loop", "photons" ]:
                continue

            good_items.append(item)
            commands += gen_to(room)

            commands += ["take " + item]

            commands += ["inv"]
            commands += gen_from(room)

    commands += gen_to("== Security Checkpoint ==")
    commands += ["inv"]
    commands += ["south"]

    ci = 0



    #s = "\x0A".join([x.strip() for x in springcode.strip().splitlines()]) + "\x0A"
    
    s = ""
    si = 0

    outline = ""
    state = State()

    stepcnt = 0 

    door_flag = False
    item_flag = False

    item_iter = 0

    while True:
        stepcnt += 1
        if stepcnt >= 10000000:
            break
        res = c.proc()
        if res == HALT:
            print("HALT")
            break
        elif res == AWAIT:
            if si >= len(s):
                #s = random.choice(["north", "east", "south", "west"])

                    
                if ci >= len(commands):
                    k = 1
                    item_iter += 1
                    commands.append("inv")
                    for item in good_items:
                        if (k & item_iter) > 0:
                            commands.append("take " + item)
                            print("TAKING ", item)
                        else:
                            commands.append("drop " + item)
                        k = k * 2
                    commands.append("inv")
                    commands.append("south")

                s = commands[ci]
                ci += 1
                state.command = s
                print("COMMAND>", s)
                s = s + "\x0A"
                si = 0
                print(s, file=log)
            c.send(ord(s[si]))        
            si += 1
        elif res is None:
            while len(c.outputs) > 0:
                code = c.outputs.pop(0)
                if (code < 256) :
                    print(chr(code), end="")
                    print(chr(code), end="", file=log)
                    if code == 10:
                        print(">>{}<<".format(outline))
                        if outline.startswith("=="):
                            state.set_location(outline)
                            door_flag, item_flag = False, False
                        elif outline.startswith("Doors here lead:"):
                            door_flag, item_flag = True, False
                        elif outline.startswith("Items here:"):
                            door_flag, item_flag = False, True
                        else:
                            if door_flag or item_flag:
                                if outline.startswith("- "):
                                    if door_flag:
                                        state.add_door(outline[2:])
                                    if item_flag:
                                        state.add_item(outline[2:])
                                else:
                                   
                                    door_flag, item_flag = False, False
                    

                        outline = ""
                        
                    else:
                        outline = outline + chr(code)
                else:
                    print("Result:", code)


    for loc in state.way:
        d = state.way[loc]
        for cmd in d:
            print(loc, cmd, d[cmd])

    for loc in state.pos:
        print(loc, state.pos[loc])

    for loc in state.path:
        print('"{}": {},'.format(loc, state.path[loc]))


    for loc in state.item:
        print('"{}": {},'.format(loc, list(state.item[loc])))

    for loc in state.door:
        print('"{}": {},'.format(loc, list(state.door[loc])))



def run():

    prgs =  open("input25.txt").read()

    log = open("log25.txt", "a")

    lst = [ int(x) for x in prgs.split(",") ]
    
    c = Proc(lst[:], 0)


    #s = "\x0A".join([x.strip() for x in springcode.strip().splitlines()]) + "\x0A"
    s = ""
    si = 0

    outline = ""
    state = State()

    stepcnt = 0 

    while True:
        stepcnt += 1
        if stepcnt >= 100000000:
            break
        res = c.proc()
        if res == HALT:
            break
        elif res == AWAIT:
            if si >= len(s):
                s = input()
                s = s + "\x0A"
                si = 0
                print(s, file=log)
            c.send(ord(s[si]))        
            si += 1
        elif res is None:
            while len(c.outputs) > 0:
                code = c.outputs.pop(0)
                if (code < 256) :
                    if code == 10:
                        if outline.startswith("=="):
                            state.set_location(outline)
                    else:
                        outline = outline + chr(code)
                    print(chr(code), end="")
                    print(chr(code), end="", file=log)
                else:
                    print("Result:", code)

def main():
    #run()
    #autorun()
    runsemi()
    #part1()
    #part2()

if __name__ == "__main__":
    main()
