#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <string>
#include <map>
#include <deque>
#include <set>

constexpr bool DEBUG = false;

using namespace std;

using VS = vector<string>;
using VPI = vector<pair<int, int>>;
using MCPI = map<char, pair<int, int>>;

VS read(const string& s)
{
    VS v;
    ifstream ifs(s, ios::in);
    while (ifs) {
        string s;
        ifs >> s;
        v.push_back(s);
    }
    return v;
}

MCPI preprocess(const VS& a)
{
    MCPI c2p;
    for (int y = 0; y < a.size(); ++y) {
        for (int x = 0; x < a[y].size(); ++x) {
            auto ch { a[y][x] };
            if ('@' <= ch && ch <= 'Z' || 'a' <= ch && ch <= 'z') {
                c2p[ch] = pair{x, y};
            }
        }
    }
    return c2p;
}

struct Node {
    int x{0};
    int y{0};
    int from_x{0};
    int from_y{0};
};


// returns number of steps or -1 if cannot reach without keys
// and keys gathered meanwhile

pair<int, string> steps_with_keys(const VS& a, int x0, int y0, int x1, int y1, string keys)
{
    auto h = a.size();
    auto w = a[0].size();

    vector<vector<pair<int, int>>> from;
    for (auto y = 0U; y < h; ++y) {
        VPI v(w, pair{0, 0});
        from.emplace_back(v);
    }

    deque<pair<int, int>> q;

    from[y0][x0] = pair{w, h};

    q.push_back({x0, y0});

    while (q.size()) {
        auto [x, y] = q.front();
        q.pop_front();

        if (DEBUG) { cout << "x:" << x << " y:" << y << endl; }

        if (x == x1 && y == y1) {
            // walk back to start using from and return
            // add keys
            pair<int, int> b{x, y};
            int steps { 0 };
            while (b.first != w) {
                char ch = a[b.second][b.first];
                if ('a' <= ch && ch <= 'z' && keys.find(ch) == string::npos) {
                    keys.push_back(ch);
                } 
                b = from[b.second][b.first];
                ++steps;
            }
            return {steps - 1, keys};
        }
        char ch = a[y][x];
       
        static const int D[] = { 1, 0, -1, 0, 1 };
        for (auto dir = 0; dir < 4; ++dir) {
            
            auto nx {x + D[dir]}, ny { y + D[dir + 1]};

            if (from[ny][nx].first != 0) {
                continue;
            }
            
            ch = a[ny][nx];
            
            if (ch == '#') {
                continue;
            }
            
            if ('A' <= ch  && ch <= 'Z') {
                auto key = ch - 'A' + 'a';
                if (keys.find(key) == string::npos) {
                    bool ok = false;
                    pair<int, int> b{x, y};
                    while (b.first != w) {
                        if (a[b.second][b.first] == key) {
                            ok = true;
                            break;
                        }
                        b = from[b.second][b.first];
                    }
                    if (!ok) {
                        continue;
                    }
                }
            }

            // can go

            from[ny][nx] = pair{ x, y };
            q.push_back({nx, ny});
            
        }
    }
    return {-1, keys}; 
}

unsigned int s2bits(const string& s)
{
    unsigned int code{0};
    for (const auto ch: s) {
        code |= 1 << (ch - 'a');
    }
    return code;
}

int main(int, char**)
{
    VS a = read("input18.txt");
    //VS a = read("input18c.txt");

    MCPI c2p { preprocess(a) };

    for (const auto& s : a) {
        cout << s << '\n';
    }

    for (const auto& pr : c2p) {
        cout << pr.first << ":" << pr.second.first << "," << pr.second.second << endl;
    }

    auto [ x0, y0 ] = c2p['@'];

    auto [ x1, y1 ] = c2p['a'];
    auto [ x2, y2 ] = c2p['b'];
    auto [ x3, y3 ] = c2p['c'];

    auto [ nsteps, nkeys ] = steps_with_keys(a, x0, y0, x1, y1, "");

    cout << nsteps << "," << nkeys << endl;

    auto [ nsteps2, nkeys2 ] = steps_with_keys(a, x1, y1, x2, y2, nkeys);
    cout << nsteps2 << "," << nkeys2 << endl;

    auto key_count = distance(c2p.find('a'), c2p.end());

    cout << "key count " << key_count << endl;

    // cost to path, keys
    multimap<int, pair<string, string>> pq;

    // path, keys
    set<pair<string, string>> impossible;

    map<pair<char, unsigned int>, int> best_at_pos_with_keys;

    pq.insert(pair{0, pair{"@", ""}});

    int min_steps = 10'000'000;

    while (!pq.empty()) {
        
        auto [ cost, pr ] = *pq.begin();
        auto [ path, keys ] = pr;

        pq.erase(pq.begin());

        cout << path << "," << keys << "," << cost << endl;

        if (keys.size() == key_count) {
            min_steps = min(cost, min_steps);
            break;
        }

        //int cost = 0;
        //string path = "@";
        //string keys = "";
        
        for (auto it = c2p.find('a'); it != c2p.end(); ++it) {
            auto prevch = path.back();
            auto [ prevx, prevy ] = c2p[prevch];
            auto [ nch, np ] = *it;
            if (keys.find(nch) == string::npos) {
                auto [ nx, ny ] = np;
                auto [ nsteps, nkeys ] = steps_with_keys(a, prevx, prevy, nx, ny, keys);
                if (nsteps >= 0) {
                    auto idx = pair{nch, s2bits(nkeys) };
                    auto prev_best_cost = best_at_pos_with_keys[idx];
                    if (prev_best_cost == 0 || cost + nsteps < prev_best_cost) { 
                        pq.insert(pair{cost + nsteps, pair{path + nch, nkeys}});
                        best_at_pos_with_keys[idx] = cost + nsteps;
                    }
                }
            }
        }
    }
    
    cout << "Min count " << min_steps << endl;

    return 0;
}

