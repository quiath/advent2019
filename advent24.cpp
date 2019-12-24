#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <bitset>


using namespace std;


using StrVec = vector<string>;
using IntVec = vector<int>;
using StrVecVec = vector<vector<string>>;

constexpr int N = 5;

constexpr int STEPS = 200;

using UInt = unsigned long;

string INPUT_MINE = R"(
.###.
##...
...##
.#.#.
#.#.#
)";

string INPUT_TEST = R"(
....#
#..#.
#..##
..#..
#....
)";

//string INPUT = INPUT_TEST;
string INPUT = INPUT_MINE;

constexpr bool DEBUG = false;

UInt s2n(const string& s)
{
    UInt b = 1;
    UInt r = 0;
    for (const char& ch : s) {
        if (ch == '#' || ch == '.') {
            if (ch == '#') {
                r |= b;
            }
            b <<= 1;
        }

    }
    return r;
}

string n2s(UInt n, UInt N)
{
    string r;
    while (r.size() < N * N) {
        auto b = (n & 1);
        r.append(1, ".#"[b]);
        n >>= 1;
    }
    return r;
}


void step_single(const string& s, string& t, int N)
{
    int i { 0 };
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            int c = s[i] == '#';
            int nc { 0 };
            if (y > 0 && s[i - N] == '#') { ++nc; };
            if (y < N - 1 && s[i + N] == '#') { ++nc; };
            if (x > 0 && s[i - 1] == '#') { ++nc; };
            if (x < N - 1 && s[i + 1] == '#') { ++nc; };
            
            if (c) {
                t[i] = ".#"[nc == 1];
            } else {
                t[i] = ".#"[nc == 1 || nc == 2];
            }
            ++i;
        }
    }
}

struct Cnt
{
    int up    { 0 };
    int down  { 0 };
    int left  { 0 };
    int right { 0 };
};

void step_with_next_levels(const string& s, string& t, int N, const Cnt& prev, const Cnt& next)
{
    int C = N / 2;
    int i { 0 };
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            int c = s[i] == '#';
            int nc { 0 };
            if (x != C || y != C) {
                if (y == C + 1 && x == C) { 
                    nc += next.down; 
                } else if (y > 0 && s[i - N] == '#') { ++nc; };
                if (y == 0) { nc += prev.down; }

                if (y == C - 1 && x == C) {
                    nc += next.up; 
                } else if (y < N - 1 && s[i + N] == '#') { ++nc; };
                if (y == N - 1) { nc += prev.up; }

                if (x == C + 1 && y == C) {
                    nc += next.right;
                } else if (x > 0 && s[i - 1] == '#') { ++nc; };
                if (x == 0) { nc += prev.right; }

                
                if (x == C - 1 && y == C) {
                    nc += next.left;
                } else if (x < N - 1 && s[i + 1] == '#') { ++nc; };
                if (x == N - 1) { nc += prev.left; }
            } 

            if (DEBUG) { cout << x << "," << y << ": neighbours=" << nc << endl; }
            
            if (c) {
                t[i] = ".#"[nc == 1];
            } else {
                t[i] = ".#"[nc == 1 || nc == 2];
            }
            ++i;
        }
    }
}


int count_borders(const string& s, int i0, int di, int n)
{
    int c { 0 };
    int index { i0 };
    for (int k  = 0; k < n; ++k) {
        c += (s[index] == '#');
        index += di;
    }
    return c;
}


UInt part1()
{
    cout << INPUT << endl;
    UInt n = s2n(INPUT);
    cout << n << endl;
    string s{ n2s(n, N) };
    string t{ s };
    cout << s << endl;

    for (auto i = 0U; i < s.size(); i += N) {
        cout << string_view(&s[i], N) << endl;
    }

    bitset<(1 << (N*N))> seen;

    while (seen[n] == 0) {
        seen[n] = 1;
        step_single(s, t, N);
        s = t;
        n = s2n(s);
    }

    cout << n << endl;

    for (auto i = 0U; i < s.size(); i += N) {
        cout << string_view(&s[i], N) << endl;
    }



    return n;
}

void print(const string& s)
{
    for (auto i = 0U; i < s.size(); i += N) {
        cout << string_view(&s[i], N) << endl;
    }
}

void part2()
{
    string ss;
    ss.append(N * N, '.');

    StrVec lev(2 * STEPS + 3, ss);
    StrVec levnew(2 * STEPS + 3, ss);

    cout << INPUT << '\n' << endl;
    UInt n = s2n(INPUT);
    cout << n << endl;
    string s{ n2s(n, N) };
    string t{ s };
    cout << s << '\n' << '\n' << endl;

    int start = STEPS + 1;
    lev[start] = s;

    print(s);

    int C = N / 2;

    for (int step = 0; step < STEPS; ++step) {
        for (int i = start - step - 1; i < start + step + 2; ++i) {
            if (DEBUG) { cout << "Computing level " << (i - start) << endl; }

            const auto& s = lev[i];

            if (DEBUG) { print(s); }

            const auto ps = lev[i - 1];
            Cnt prev {
                int(ps[(C + 1) * N + C] == '#'),
                int(ps[(C - 1) * N + C] == '#'),
                int(ps[C * N + C + 1] == '#'),
                int(ps[C * N + C - 1] == '#')
            };
            if (DEBUG) { cout << "UDLR " << prev.up << ',' << prev.down << ',' << prev.left << ',' << prev.right << endl; }

            int k = i + 1;
            const auto& ks = lev[k];
            int up = count_borders(ks, 0, 1, N);
            int left = count_borders(ks, 0, N, N);
            int right = count_borders(ks, N - 1, N, N);
            int down = count_borders(ks, N * N - N, 1, N);
            Cnt next{up, down, left, right};

            step_with_next_levels(s, levnew[i], N, prev, next);

            if (DEBUG) {
                cout << "After step " << step << " level " << (i - start) << endl;
                print(levnew[i]);
            }

        }
        lev = levnew;
        if (DEBUG) {
            int ret { 0 };
            for (const auto& s : lev) {
                ret += count(s.begin(), s.end(), '#');
            }

            cout << ret << endl;
        }
    }

    int ret { 0 };
    for (const auto& s : lev) {
        ret += count(s.begin(), s.end(), '#');
    }

    cout << ret << endl;


}

int main(int, char**)
{
    part1();

    part2();

    return 0;

}
