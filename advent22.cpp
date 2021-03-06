#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>


using namespace std;


using StrVec = vector<string>;
using IntVec = vector<int>;
using StrVecVec = vector<vector<string>>;


StrVecVec read(const string& fn)
{
    StrVecVec v;
    ifstream ifs(fn);
    string s;
    while (getline(ifs, s)) {
        istringstream iss(s);
        StrVec line_tokens;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(line_tokens));
        v.push_back(line_tokens);
    }
    return v;
}

// straightforward shuffle of the table 

void part1()
{


    auto inp = read("input22.txt");
    const int N = 10007;

    
    IntVec a(N);
    for (auto i = 0; i < N; ++i) {
        a[i] = i;
    }

    for (const auto& line_tokens : inp) {
        copy(line_tokens.begin(), line_tokens.end(), ostream_iterator<string>(cout, "  "));
        cout << endl;
    }


    for (const auto& line_tokens : inp) {
        const auto&s { line_tokens[0] };
        if (s == "deal") {
            if (line_tokens[1] == "into") {
                reverse(a.begin(), a.end());
                continue;
            }
            int val = stoi(line_tokens[3]);
            IntVec b(N);
            int k = 0;
            for (int i = 0; i < N; ++i) {
                b[k] = a[i];
                k += val;
                k = k % N;
            }
            a = std::move(b);
            continue;
        }
        if (s == "cut") {
            int val = stoi(line_tokens[1]);
            if (val > 0) {
                rotate(a.begin(), a.begin() + val, a.end()); 
            } else if (val < 0) {
                rotate(a.begin(), a.begin() + (N + val), a.end()); 
            }
            continue;
        }
    }

    auto it = find(a.begin(), a.end(), 2019);
    cout << "Part 1 result: " << distance(a.begin(), it) << endl;
    cout << "Test pos 2019: " << a[2019] << endl;
    cout << "Test pos 2020: " << a[2020] << endl;

}

// 64 bit overflowed!!!
// using UInt = unsigned long long;
// using Int = long long;
using UInt = unsigned __int128_t;
using Int = __int128_t;

using IntC = int64_t;

// used for testing some theories about the structure
//#define TEST
#ifdef TEST

constexpr UInt N = 10007ULL;

constexpr UInt START = 2019ULL;
constexpr UInt M = 10007ULL;

constexpr UInt R = 2514ULL;

#else

constexpr UInt N = 119315717514047ULL;
constexpr UInt START = 0ULL;

constexpr UInt M = 101741582076661ULL;

#endif

// operations on indexes for each operation

UInt rev(UInt k)
{
    return N - 1 - k;
}


// Inverse

UInt rev_1(UInt k)
{
    return N - 1 - k;
}

UInt rot(UInt k, Int val)
{
    if (val > 0) {
        return (k + N - val) % N;
    }
    return rot(k, N + val);
}

// Finding identities about operations

// cut 3
// 0 1 2 3 4 5 6
// 3 4 5 6 0 1 2

// cut-1 3 == cut 4
// 0 1 2 3 4 5 6
// 4 5 6 0 1 2 3

// cut -4 

// 0 1 2 3 4 5 6
// 3 4 5 6 0 1 2

// cut-1 -4 == cut 3
// 0 1 2 3 4 5 6
// 4 5 6 0 1 2 3

// Inverse rot

UInt rot_1(UInt k, Int val)
{
    if (val > 0) {
        return rot(k, N - val);
    }
    //return rot_1(k, N + val);
    return rot(k, -val);
}


UInt rot(UInt k, Int val, UInt N)
{
    if (val > 0) {
        return (k + N - val) % N;
    }
    return rot(k, N + val, N);
}

UInt rot_1(UInt k, Int val, UInt N)
{
    if (val > 0) {
        return rot(k, N - val, N);
    }
    //return rot_1(k, N + val, N);
    return rot(k, -val, N);
}



// deal into 3
// 0 1 2 3 4 5 6 
// 0 5 3 1 6 4 2

// (0)(531642)
// (246135)(0)

// 0 3 6 2 5 1 4
// deal-1 3 == deal into 5 (N + 1 - val) ???


// deal into 1 == deal-1 into 1
// 0 1 2 3 4 5 6 
// 0 1 2 3 4 5 6

// deal into 2
// 0 1 2 3 4 5 6 
// 0 4 1 5 2 6 3

// deal-1 2 == deal 4
// 0 2 4 6 1 3 5

// 1 1
// 2 4
// 3 5
// 4 2 
// 5 3
// 

// 0 1 2 3 4 5 6
// 0 6 5 4 3 2 1
// deal-1 6 = deal 6


// 1 5 4 6 2 3 

// new_full = (old * val)
// new = (old * val) % N
// new + j * N = old * val
// old = (new + j * N) / val

// 1 3 1 

// 0 1 2 3 4 5 6 7 8 9
// 0 7 4 1 8 5 2 9 6 3

// 3 1 7 9 3
// 2 4 8 6 2


// 0 1 2 3 4
// 0 3 1 4 2
// 0 2 4 1 3
// 0 4 3 2 1

// 0 1 2 3 4 5 6 
// 0 4 1 5 2 6 3
// 0 5 3 1 6 4 2
// 0 2 4 6 1 3 5
// 0 3 6 2 5 1 4
// 0 6 5 4 3 2 1

// 3 * k = j * 7 + 1

// 3 * k == 1 (mod 7)
// 7 | 3 * k - 1

// when trying to inverse deal_into I discovered myself that I need to 
// do the inverse modulo
// for which I found an algorithm online

// Source: https://www.geeksforgeeks.org/modular-division/

// C function for extended Euclidean Algorithm 
Int gcdExtended(Int a, Int b, Int& x, Int& y); 
  
// Function to find modulo inverse of b. It returns 
// -1 when inverse doesn't 
Int modInverse(Int b, Int m) 
{ 
    Int x, y; // used in extended GCD algorithm 
    Int g = gcdExtended(b, m, x, y); 
  
    // Return -1 if b and m are not co-prime 
    if (g != 1) 
        return -1; 
  
    // m is added to handle negative x 
    return (x%m + m) % m; 
} 
  
// Function to compute a/b under modulo m 
void modDivide(Int a, Int b, Int m) 
{ 
    a = a % m; 
    Int inv = modInverse(b, m); 
    if (inv == -1) 
       cout << "Division not defined"; 
    else
       cout << "Result of division is " << IntC((inv * a) % m); 
} 

Int modDiv(Int a, Int b, Int m) 
{ 
    a = a % m; 
    Int inv = modInverse(b, m); 
    if (inv == -1) {
        return -1LL;
    }
    return (inv * a) % m; 
} 
 
// C function for extended Euclidean Algorithm (used to 
// find modular inverse. 
Int gcdExtended(Int a, Int b, Int& x, Int& y) 
{ 
    // Base Case 
    if (a == 0) 
    { 
        x = 0;
        y = 1; 
        return b; 
    } 
  
    Int x1, y1; // To store results of recursive call 
    Int gcd = gcdExtended(b%a, a, x1, y1); 
  
    // Update x and y using results of recursive 
    // call 
    x = y1 - (b/a) * x1; 
    y = x1; 
  
    return gcd; 
} 




UInt deal_into(UInt k, Int val)
{
    return (val * k) % N; 
}

// most difficult inverse

UInt deal_into_1(UInt k, Int val)
{
    if (k == 0) {
        return 0;
    }
    Int inv = modDiv(1, val, N);
    if (inv < 0) {
        cout << IntC(k) << "," << IntC(val) << endl;
        throw 42;
    }
    UInt uval = inv;
    return (uval * k) % N; 
}


UInt deal_into(UInt k, Int val, UInt N)
{
    return (val * k) % N; 
}

UInt deal_into_1(UInt k, Int val, UInt N)
{
    if (k == 0) {
        return 0;
    }
    Int inv = modDiv(1, val, N);
    if (inv < 0) {
        cout << IntC(k) << "," << IntC(val) << endl;
        throw 42;
    }
    UInt uval = inv;
    return (uval * k) % N; 
}

// prefer these aliases to even shorter abbreviations

using IntPair = pair<int, int>;
using IntPairVec = vector<IntPair>;

// translation of the text commands to int commands, for speed

IntPairVec translate(const StrVecVec& inp)
{
    IntPairVec result;
    for (const StrVec& line_tokens : inp) {
        const string& s { line_tokens[0] };
        if (s == "deal") {
            if (line_tokens[1] == "into") {
                result.push_back(pair{0, 0});
            } else {
                int val = stoi(line_tokens[3]);
                result.push_back(pair{1, val});
            }
        } else if (s == "cut") {
            int val = stoi(line_tokens[1]);
            result.push_back(pair{2, val});
        }
    }
    return result;
}

// the best I could find myself for part 2 was to create 
// functions that compute the forward shuffle function
// and backward (inverse shuffle)
// this was not enough
// the key was the fact that multiple applications
// of a linear function ax+b is a linear function Ax+B

Int forward(const IntPairVec& inp, Int x)
{
    for (const auto& p : inp) {
        auto [cmd, val] = p;
        switch (cmd) {
            case 0:
                x = rev(x);                
                break;
            case 1: 
                x = deal_into(x, val);
                break;
            case 2:
                x = rot(x, val);
        }
    }
    return x;
}

Int backward(const IntPairVec& inp, Int x)
{
    for (auto it = inp.rbegin(); it != inp.rend(); ++it) {
        auto [cmd, val] = *it;
        switch (cmd) {
            case 0:
                x = rev_1(x);                
                break;
            case 1: 
                x = deal_into_1(x, val);
                break;
            case 2:
                x = rot_1(x, val);
        }
    }
    return x;
}


void part2()
{
    auto inp = read("input22.txt");

    auto num_input { translate(inp) };

    // once I knew I had to compute the A,B 
    // to express the entire set of transformations 
    // as Ax+B, I arrived at this solution
    // having A and B it was moderately easy 
    // to compute A0, B0 for A0x+B0 expressing
    // the entire reverse set of transformations

    Int B = forward(num_input, 0);
    Int A = (forward(num_input, 1) + N - B) % N;
    cout << "A B " << IntC(A) << " " << IntC(B) << " nxt " << IntC((A * 2019 + B) % N) <<  endl;
    Int B0 = modDiv(N - B, A, N);
    Int T = modDiv(N + 1 - B, A, N);
    Int A0 = (N + T - B0) % N;
    cout << "A0 B0 " << IntC(A0) << " " << IntC(B0) << " nxt " << IntC((A0 * 2514 + B0) % N) <<  endl;

    /*
A B 4166808150734 91211973193983 nxt 32581686028592
A0 B0 115432309303926 105522863498059 nxt 7207741891672
    */
    
    {
        // here's the other clue I used
        // f(f(x)) = f(a*x+b) = a*(a*x+b)+b = a*a*x+a*b+b => a1=a*a, b1=a*b*b
        // where mod N can be applied at every step
        // I knew the fast power computation algorithm earlier and applied it

        Int x = 2020;
        Int y = M;
        Int a = A0;
        Int b = B0;

        while (y > 0) {
            Int bit = y & 1;
            y >>= 1;
            if (bit) {
                x = (a * x + b) % N;
            }
            Int na = (a * a) % N;
            Int nb = ((a * b) + b) % N;
            a = na;
            b = nb;
        }
        cout << "Result part 2: " << IntC(x) << endl;
    }
    
}

int main(int, char**)
{
    part1();

    part2();

    return 0;

}
