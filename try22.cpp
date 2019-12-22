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

//using UInt = unsigned long long;
//using Int = long long;
using UInt = unsigned __int128_t;
using Int = __int128_t;

using IntC = int64_t;

#define TEST
#ifdef TEST


constexpr UInt N = 10007ULL;
//constexpr UInt N = 10009ULL;

constexpr UInt START = 2019ULL;
//constexpr UInt M = 10007ULL;
constexpr UInt M = N;

constexpr UInt R = 2514ULL;

#else

constexpr UInt N = 119315717514047ULL;
constexpr UInt START = 0ULL;

constexpr UInt M = 101741582076661ULL;

#endif

UInt rev(UInt k)
{
    return N - 1 - k;
}


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


//0123456
//3502461

//0123456
//2630415

//0123456
//6012345 (N-1)

//
// b = 2 * k + 3 (mod 7)
// 
// k = 
//

using IntPair = pair<int, int>;
using IntPairVec = vector<IntPair>;

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
    cout << IntC(Int(-1)) << endl;
    
    Int mod = 7;
    for (int i = 1; i <= 6; ++i) {
        cout << "Divide 1 by " << i << " mod " << IntC(mod) << endl;
        modDivide(1, i, mod);
        cout << endl;
        
        UInt c { 2 };
        auto nc = /*deal_into*/rot(c, -i, mod);
        auto pc = /*deal_into_1*/rot_1(nc, -i, mod);
        cout << IntC(c) << "," << IntC(nc) << "," << IntC(pc) << endl;
    }


    auto inp = read("input22.txt");

    auto num_input { translate(inp) };

#ifdef TEST    
    UInt testval = forward(num_input, 2019);
    cout << "Forward returned:" << IntC(testval) << endl;
    UInt rval = backward(num_input, testval);

    cout << "Backward returned:" << IntC(rval) << endl;
#endif

    cout << "0:" << IntC(forward(num_input, 0)) << endl;
    cout << "1:" << IntC(forward(num_input, 1)) << endl;
    cout << "2:" << IntC(forward(num_input, 2)) << endl;
    cout << "3:" << IntC(forward(num_input, 3)) << endl;
    cout << "4:" << IntC(forward(num_input, 4)) << endl;
    cout << "" << IntC(forward(num_input, 4)) - IntC(forward(num_input, 3)) << endl;
    cout << "" << IntC(forward(num_input, 1)) - IntC(forward(num_input, 0)) << endl;

    cout << (311 * 2019 + 5046) % 10007 << endl;
    cout << IntC(modDiv(10007 - 5046, 311, N)) << endl;

    Int B = forward(num_input, 0);
    Int A = (forward(num_input, 1) + N - B) % N;
    cout << "A B " << IntC(A) << " " << IntC(B) << " nxt " << IntC((A * 2019 + B) % N) <<  endl;
    Int B0 = modDiv(N - B, A, N);
    Int T = modDiv(N + 1 - B, A, N);
    Int A0 = (N + T - B0) % N;
    cout << "A0 B0 " << IntC(A0) << " " << IntC(B0) << " nxt " << IntC((A0 * 2514 + B0) % N) <<  endl;

    return;

    // forward: x -> 311 * x + 5046
    // forward: x -> A   * x + B
    // backward: x -> A0 * x + B0
    // B0 = modDiv(N - B, A, N)
    // T = modDiv(N + 1 - B, A, N)
    // A0 = N + T - B0

    // 9122 + x * 3845


    //Int start { M / 2 }; // nothing
    //Int start { M / 2 + 1 }; // nothing
    //Int start { N / 2 };  // nothing
    // Int start { N - 1 };
    // Int start { N - M };

    Int start { 2019 };

    //Int tgt { 2020 };
    Int tgt { 2019 };
    Int fwd{start}, bck{start};
    for (auto i = 0LL; i < M; ++i) {
        if (i % 1000000 == 0) {
            cout << "i=" << i << " before fwd=" << IntC(fwd) << " bck=" << IntC(bck) << "\n" ;
        }
        auto fwdn = forward(num_input, fwd);
        auto bckn = backward(num_input, bck);
        
        if (fwdn == tgt || bckn == tgt) {
            cout << "i=" << i << " after fwd=" << IntC(fwdn) << " <- " << IntC(fwd) << " bck=" << IntC(bckn) << " <- " << IntC(bck) << "\n" ;
        }
        //cout << IntC(fwdn - fwd) << "," << IntC(bckn - bck) << '\n';
        fwd = fwdn;
        bck = bckn;

    }

    return;


    /*
    for (const auto& line_tokens : inp) {
        copy(line_tokens.begin(), line_tokens.end(), ostream_iterator<string>(cout, "  "));
        cout << endl;
    }
    */

    //UInt x {2020ULL};
    //UInt x {START};
    UInt x {2307};

    UInt orig { START };

    for (UInt iter = 0; iter < M; ++iter) {
        auto px { x };
        for (const auto& line_tokens : inp) {
            const auto&s { line_tokens[0] };
            if (s == "deal") {
                if (line_tokens[1] == "into") {
                    auto nx = rev(x);
                    auto x_1 = rev_1(nx);
                    if (x != x_1) {
                        cout << "rev! " << IntC(x) << endl; 
                    }
                    x = nx;
                    continue;
                }
                int val = stoi(line_tokens[3]);
                auto nx = deal_into(x, val);
                auto x_1 = deal_into_1(nx, val);

                if (x != x_1) {
                    cout << "deal! " << IntC(x) << " nx=" << IntC(nx) << " x_1=" << IntC(x_1) << endl; 
                }

                x = nx;
            } else if (s == "cut") {

                int val = stoi(line_tokens[1]);
                auto nx = rot(x, val);
                auto x_1 = rot_1(nx, val);

                if (x != x_1) {
                    cout << "rot! x=" << IntC(x) << " nx=" << IntC(nx) << " x_1=" << IntC(x_1) << " val=" << val << endl; 
                }

                x = nx;
            }
        }
        if (x == orig) {
            cout << "Match after iteration " << IntC(iter) << " " << IntC(px) << " -> " << IntC(x) << endl;
        }
        if (px == orig) {
            cout << "After iteration " << IntC(iter) << " " << IntC(px) << " -> " << IntC(x) << endl;
        }

        //cout << iter << " " << x << endl;
    }

    // after 1 it. a[2019] = 2307
    // predecessor(2019) = 2307

    cout << "Test part 2: " << IntC(x) << endl;

    auto nx = 2019; //R;
    for (Int iter = 0; iter < /*M*/ 1; ++iter) {
        for (int i = inp.size() - 1; i >= 0; --i) {
            const auto& line_tokens = inp[i];
            const auto& s { line_tokens[0] };
            if (s == "deal") {
                if (line_tokens[1] == "into") {
                    auto x_1 = rev_1(nx);
                    nx = x_1;
                    continue;
                }
                int val = stoi(line_tokens[3]);
                auto x_1 = deal_into_1(nx, val);

                nx = x_1;
            } else if (s == "cut") {

                int val = stoi(line_tokens[1]);
                auto x_1 = rot_1(nx, val);

                nx = x_1;
            }
        }
        /*if (x == orig) {
            cout << "Match after iteration " << IntC(iter) << " " << IntC(px) << " -> " << IntC(x) << endl;
        }
        if (px == orig) {
            cout << "After iteration " << IntC(iter) << " " << IntC(px) << " -> " << IntC(x) << endl;
        }*/

        cout << IntC(iter) << " predecessor " << IntC(nx) << endl;
    }



}

int main(int, char**)
{
    part1();

    part2();

    cout << "119315717514047" << endl; // prime
    cout << "101741582076661" << endl; // prime

    return 0;

}
