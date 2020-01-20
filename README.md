# advent2019
Advent of Code 2019

My quick and dirty solutions to Advent of Code in year 2019.


**Warning, spoilers below**

All the source files are in this directory in the format `advent{day:02d}.{extension}`.

## Day 1: The Tyranny of the Rocket Equation

Simple computations in Python according to the given equations.

## Day 2: 1202 Program Alarm

I'm starting to build the IntCode computer in Python. For now only addition and multiplication are needed.

The second part is just brute forcing the solution.

## Day 3: Crossed Wires 

### Finding the intersection point closes to origin between wires on a 2D grid. 

Solved using a Python sets to represents grid coordinates. Set intersections elegantly give me the wire intersections. Later, minimum distance of origin is computed.

### Finding the intersection point reachable in fewest steps

A trivial modification of the previous algorithm: intersections are also checked at every step when building the coordinate sets.
The first intersection found is the one that required the fewest steps.


## Day 4: Secure Container

Javascript solution for a change, built using Array.from(), filter(). Second part required only a slightly modified condition.

## Day 5: Sunny with a Chance of Asteroids

Python solution, IntCode computer continued. Part 1 is 2 new opcodes and addressing modes. 

Part 2 adds conditionals. IntCode computer is implemented by just a couple of Python functions. No big challenges here apart from getting the addressing modes right.

## Day 6: Universal Orbit Map

This one I solved using Lua (actually on an Ipad using the Codea program). 

I embedded the input data in the code thanks to Lua long strings.
The parser uses Lua regular expressions. 

The first problem is equivalent to walking the entire tree, which I did recursively in the walk() function.

The second problem requires finding the common parent of two particular nodes in the tree, which I implemented 
by storing the first special node after encountering it and subsequently computing the parent and the distance 
after finding the other special node.

## Day 7: Amplification Circuit

Part 1 required generating permutations which is trivial with itertools in Python.

The second part required simultaneous running of several IntCode instances. 
This looked like it might be a good use case for Python coroutines. Unfortunately this approach failed and I lost a lot of time.
Instead I used the well tried approach with a class encapsulating the IntCode computer and its state.
I'm still wondering if the coroutines might be a good fit here.

This was the first challenging task in 2019, I think, but technically and not algorithmically.

## Day 8: Space Image Format

Solved in C++. The first part required counting the number of characters of reach type (trivial). 

The second part was to get the resulting image built from multiple layers, including transparency.
The solution was to find the first visible pixel in the stack at each coordinate.

## Day 9: Sensor Boost

Part 1 added the relative addressing mode for the IntCode. Time consuming since it was easy to make a mistake 
when decoding the different addressing modes. As a reward for my time I got the complete IntCode computer.

Part 2 required just running the program on the supposedly correct IntCode.

As all my IntCode solutions this was written in Python.

## Day 10: Monitoring Station  

Part 1 required counting visible asteroids on an integer grid. Asteroids could block one another. 
My solution was to look at the locations at "edges of a square" centered on the monitoring station position,
starting with the smallest square and increasing it by 1 in each direction. Each encountered asteroid was
counted and then all asteroids along its line were erased (as they were effectively not visible and could not 
be counted).

Part 2 was a more challenging task since it required sorting the grid points in the order of the angle
from the laser position. I made this more challenging than needed because I used Python fractions library
and computed everything without trigonometry, atan() etc. Unfortunately this required dividing the grid 
into 8 octants, each with a different ordering rule. This in turn led to a few small mistakes in the ordering,
one of which manifested very later in the processing.

Here is my comment for the main part:
```
divide the board into 8 octants (8 triangles)
create a list of asteroids starting with the octants 
sort asteroids in each octant by the ratio of x offset to y offset (negated or reciprocal as appropriate) 
carefully include points on the vertical, horizontal and diagonal lines - only once! 
```

## Day 11: Space Police 

Both parts required simply hooking up the IntCode computer via a system o input/output commands to a 2D array which was
"painted". Relatively easy.

## Day 12: The N-Body Problem 

Solution in Lua. 

The first part, a simulation of a 3D "integer physics" system was pretty basic and required writing a few loops. 

The second part was tricky! It asked when will the first repeated state happen. But it is impossible to find the answer 
by running the simulation, the number of steps is too high.

It was easy to notice that in the 3D "integer physics" system each position was computed totally independently. 
So it was enough to determine the first repeated position for each of the dimensions. 
Finally I wrote a least common multiplier (LCM) routine that gave me the answer.

Solving this day was not challenging technically, but it required some mathematical skills and above most intuition.

## Day 13: Care Package

Best fun in my third year with Advent of Code. 

Part 1 is simply about hooking up display to the IntCode arcade cabinet. 

Part 2 is much more challenging. Not only it requires providing inputs and reading output. You must design your program 
to play on the IntCode computer and win.

I was mesmerized when my program started moving the paddle correctly and played the game to the finish!

But, frankly, the paddle control did not require any advanced algorithm.

## Day 14: Space Stoichiometry 

Solved in C++. 

For Part 1 I needed significant time just to write the parser. There is a "rule system", i.e. some linear 
transformations between vectors. Part 1 required just a greedy algorithm to apply these transformations. 

Part 2 was more complicated since I had to compute the inverse of the find() function for a given result. 
My solution was to assume that find() was monotonic and find the desired input using interpolation and then iteration.
This was not quite straightforward and I suspect there was a simpler way using some mathematical properties
of the function I computed.

## Day 15: Oxygen System

Solved in Python as it was an IntCode puzzle. 

This time we had to interface with the IntCode computer that was running a droid in a labirynth.

For the first path I implemented Breadth First Search. It was not very efficient since I returned the droid
to the starting location after visiting each new location. However this greatly simplified the algorithm and state keeping.

The second part of the puzzle required finding the most distant node in the graph from a given source, 
so I reused the same proven BFS routine. The entire puzzle was relatively straightforward.

## Day 16: Flawed Frequency Transmission 

Part 1 was pretty basic. Solved in Python.

Part 2 made the problem much harder as it was impossible to simulate the entire the result of the function given the huge 
input data. However after much staring at the outputs of the function for smaller inputs I found that the second 
part of the output can be computed much faster, and the entire answer was embedded in the second half.

I heard that this puzzle was difficult to some people. I was able to solve it without external help due to the
visualization I added. But maybe this was really too "tricky"?

## Day 17: Set and Forget 

Again solved in Python. Part 1 involved interfacing with the IntCode computer to get a list of directions and steps in that directions.

The second part required compressing the text that was the answer to the first part. In fact I compressed it by hand. Easy.

## Day 18: Many-Worlds Interpretation 

A very interesting puzzle which I solved in C++. 

Part 1 requires solving the shortest paths visiting all keys in a labirynth with keys and doors, 
where you cannot pass a door without a corresponding key. 

Notice that you only need to travel between positions of the keys. And the keys you hold need to be a part 
of your state. I created a function steps_with_keys():
```
// returns number of steps or -1 if cannot reach without keys
// and keys gathered meanwhile
```
My search was equivalent to a search in a graph whose nodes were the last visited key position and the set of held keys.
I only needed to search it using a priority queue ordered by the cost: the number of steps so far. 

After such complicated part 1, part 2 added some concurrency, but was solved without much difficulty.

## Day 19: Tractor Beam

Another IntCode puzzle, the Python program for this one was relatively simple and involved first counting on a 2D grid 
and for the second part tracing a shape on a 2D grid.

## Day 20: Donut Maze

Solved in C++, so reading the input data was a chore compared to Python.

The part 1 was about finding the shortest path in a labirynth with portals, so I used a priority queue 
with cost being the number of steps again. 

Part 2 added recursion (multiple levels) to the labirynth, where going into inner portals increased the labirynth level,
and the outer portals decreased it. The distance() function from the first part could still be reused after minimal changes.
So I based solving part 2 on treating it as a graph of nodes where each node represented the portal and level.
In this virtual graph I found the shortest path from the initial portal at level 0 to the final portal at level 0. 
The cost of travel between portals was provided by distance().

This was quite complicated but I was able to reuse similar ideas as in day 18. 

## Day 21: Springdroid Adventure 

Another interesting puzzle, solved in Python. IntCode computer was used to run even more primitive program
that controlled a droid jumping over patterns of holes.

Both part 1 and part 2 required tricks. It was very frustrating to watch your droid fail.
The tricks are documented in the code and were the result of many failed attempts and observing
the hole patterns that were not passable by the previous version. 

Still it was very rewarding to find the solution to the 2nd part.

## Day 22: Slam Shuffle 

Day 22 had the most frustrating Advent of Code puzzle ever. Part 1 was relatively straightforward, if tedious.
Unfortunately I used C++ and had to resort to int128 to avoid overflow. 

Part 2 increased the size of the input so again a trick was required to solve it. 
Here is my comment:
```
// the best I could find myself for part 2 was to create 
// functions that compute the forward shuffle function
// and backward (inverse shuffle)
// this was not enough
// the key was the fact that multiple applications
// of a linear function ax+b is a linear function Ax+B
```
So even though I was able to compute the inverse of a list of functions given in the input, 
there was no periodicity in the results and I was stuck. The hint I found online was that
I only had to compute the entire transformation function, then compute the inverse.
```
        // here's the other clue I used
        // f(f(x)) = f(a*x+b) = a*(a*x+b)+b = a*a*x+a*b+b => a1=a*a, b1=a*b*b
        // where mod N can be applied at every step
        // I knew the fast power computation algorithm earlier and applied it
```
Thus after much frustration I arrived at the correct result.

## Day 23: Category Six

This was a simulation of multiple IntCode computers sending messages from one to another.
There was nothing algorithmically demanding. The only really difficult part was correctly 
handling I/O of each computer. Solved in Python.

## Day 24: Planet of Discord 

Solved in C++. Part 1 was similar to Game of Life and was very simple. 

The second part added another dimension to the puzzle and made it challenging. 
However the challenge was not really in the algorithm but in getting all the signs and variables right.
So I lost a lot of time searching for bugs in a pretty straightforward program. 

## Day 25: Cryostasis

Another cool puzzle using IntCode: old style text adventure with items and pressure sensitive floor that
only let you through if you carried the right items! And some items made you lose the game!

Instead of using any real graph search routine I simply applied random walk and updated the list
of "do not touch" items manually. Finally after getting to the "Pressure-Sensitive Floor"
my code simply brute forced all the item combinations to complete Advent of Code 2019.

**All days were completed within 21 hours each**



