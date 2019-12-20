#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <string>
#include <map>
#include <deque>
#include <set>
#include <algorithm>

constexpr bool DEBUG = false;

using namespace std;

using VS = vector<string>;
using PI = pair<int, int>;
using VPI = vector<pair<int, int>>;
using MCPI = map<char, pair<int, int>>;
using MIPI = map<int, pair<int, int>>;
using MSPI = map<string, pair<int, int>>;
using UI = unsigned int;

VS read(const string& fn)
{
    VS v;
    ifstream ifs(fn);
    string s;
    while (getline(ifs, s)) {
        v.push_back(s);
    }
    return v;
}


MSPI preprocess(const VS& a)
{
    MSPI c2p;
    int h = a.size();
    int w = a[0].size();
    
    auto add = [&](string s, int x, int y) {
        //if (c2p.find(s) != c2p.end()) {
        if (x > 2 && y > 2 && x < w - 3 && y < h - 3) {
            s.push_back('0');
        }
        c2p[s] = pair{ x, y };

    };

    for (int y = 0; y < a.size() - 1; ++y) {
        for (int x = 0; x < a[y].size() - 1; ++x) {
            auto ch { a[y][x] };
            if (isupper(ch)) {
                auto ch2 { a[y + 1][x] };
                auto ch3 { a[y][x + 1] };

                if (isupper(ch2)) {
                    string sv(1, ch);
                    sv.push_back(ch2);
                    if (y > 0 && a[y - 1][x] == '.') {
                       add(sv, x, y - 1);
                       continue;
                    }
                    if (y + 2 < a.size() && a[y + 2][x] == '.' ) {
                       add(sv, x, y + 2);
                       continue;
                    }
                } else if (isupper(ch3)) {
                    string sh(1, ch);
                    sh.push_back(ch3);
                    if (x > 0 && a[y][x - 1] == '.') {
                       add(sh, x - 1, y);
                       continue;
                    }
                    if (x + 2 < a[y].size() && a[y][x + 2] == '.' ) {
                       add(sh, x + 2, y );
                       continue;
                    }
                }

            }
        }
    }
    return c2p;
}

pair<MCPI, vector<string>> preprocess_part2(const VS& a)
{
    MCPI c2p;
    VS v(4, "");
    for (int y = 0; y < a.size(); ++y) {
        for (int x = 0; x < a[y].size(); ++x) {
            int part { int(x > a[y].size() / 2) + 2 * (y >= a.size() / 2) };
            auto ch { a[y][x] };
            if (ch == '@') {
                ch = "0123"[part];
                c2p[ch] = pair{x, y};
            } else if ('A' <= ch && ch <= 'Z' || 'a' <= ch && ch <= 'z') {
                c2p[ch] = pair{x, y};
                if ('a' <= ch && ch <= 'z') {
                    v[part] += ch;
                }
            }
        }
    }
    return { c2p, v };
}



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

UI s2bits(const string& s)
{
    UI code{0U};
    for (const auto ch: s) {
        code |= 1 << (ch - 'a');
    }
    return code;
}
#if 0
void part1()
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

    
    auto key_count = distance(c2p.find('a'), c2p.end());

    cout << "key count " << key_count << endl;

    // cost to last key, gathered keys
    multimap<int, pair<char, string>> pq;

    map<pair<char, unsigned int>, int> best_at_pos_with_keys;

    pq.insert(pair{0, pair{'@', ""}});

    int min_steps = 10'000'000;

    while (!pq.empty()) {
        
        auto [ cost, pr ] = *pq.begin();
        auto [ prevch, keys ] = pr;

        pq.erase(pq.begin());

        if (DEBUG) { cout << prevch << "," << keys << "," << cost << endl; }

        if (keys.size() == key_count) {
            min_steps = cost;
            break;
        }

        for (auto it = c2p.find('a'); it != c2p.end(); ++it) {
            auto [ nch, np ] = *it;
            if (keys.find(nch) == string::npos) {
                auto [ prevx, prevy ] = c2p[prevch];
                auto [ nx, ny ] = np;
                auto [ nsteps, nkeys ] = steps_with_keys(a, prevx, prevy, nx, ny, keys);
                if (nsteps >= 0) {
                    auto idx = pair{nch, s2bits(nkeys) };
                    auto prev_best_cost = best_at_pos_with_keys[idx];
                    if (prev_best_cost == 0 || cost + nsteps < prev_best_cost) { 
                        pq.insert(pair{cost + nsteps, pair{ nch, nkeys}});
                        best_at_pos_with_keys[idx] = cost + nsteps;
                    }
                }
            }
        }
    }
    
    cout << "Min count " << min_steps << endl;


}

void part2()
{
    VS a = read("input18_2.txt");

    auto [ c2p, part_keys] = preprocess_part2(a);

    for (const auto& s : a) {
        cout << s << '\n';
    }

    for (const auto& pr : c2p) {
        cout << pr.first << ":" << pr.second.first << "," << pr.second.second << endl;
    }

    for (auto s : part_keys) {
        cout << s << endl;
    }

    auto key_count = distance(c2p.find('a'), c2p.end());

    cout << "key count " << key_count << endl;

    // cost to positions [4],  keys. paths are not used!
    multimap<int, pair<string, string>> pq;

    map<pair<string, UI>, int> best_at_pos_with_keys; // for 4 pos

    pq.insert(pair{0, pair{"0123", ""}});

    int min_steps = 10'000'000;

    while (!pq.empty()) {
        
        auto [ cost, pr ] = *pq.begin();
        auto [ last_pos, keys ] = pr;

        pq.erase(pq.begin());

        if (DEBUG) { cout << last_pos << "," << keys << "," << cost << endl; }

        if (keys.size() == key_count) {
            min_steps = min(cost, min_steps);
            break;
        }

        
        for (auto k = 0; k < 4; ++k) {
            for (auto key : part_keys[k]) {
                if (keys.find(key) == string::npos) {
                    auto prevch = last_pos[k];
                    auto [ prevx, prevy ] = c2p[prevch];
                    auto [ nx, ny ] = c2p[key];
                    auto [ nsteps, nkeys ] = steps_with_keys(a, prevx, prevy, nx, ny, keys);
                    if (nsteps >= 0) {
                        string new_pos(last_pos);
                        new_pos[k] = key;
                        auto idx = pair{new_pos, s2bits(nkeys) };
                        auto prev_best_cost = best_at_pos_with_keys[idx];
                        if (prev_best_cost == 0 || cost + nsteps < prev_best_cost) { 
                            pq.insert(pair{cost + nsteps, pair{new_pos, nkeys}});
                            best_at_pos_with_keys[idx] = cost + nsteps;
                        }


                    }

                }
            }

        }

    }

    cout << "Min count " << min_steps << endl;

}
#endif

string other(string s)
{
    if (s.size() == 2) {
        return s + '0';
    }
    return s.substr(0, 2);
}


int distance(const VS& a, MSPI s2p, string from, string to, bool use_portals = false)
{
    VS visited;
    VS name;

    for (const auto& s : a) {
        string scopy(s);
        replace(scopy.begin(), scopy.end(), '.', char(125));
        visited.push_back(scopy);
        //cout << scopy << endl;
    }

    //MSPI s2p = preprocess(a);

    //cout << s2p.size() << endl;

    int cnt = 0;
    for (const auto& sp : s2p) {
        const auto& [ s, pos ] = sp;
        //cout << s << ":" << pos.first << "," << pos.second << endl;
        visited[pos.second][pos.first] = char(cnt++);
        name.push_back(s);
    }

    auto posaa = s2p[from];
    auto [x0, y0] = posaa;
    auto poszz = s2p[to];
    auto [x1, y1] = poszz;

    struct Node { int x{0}; int y{0}; int cost{0}; };

    deque<Node> q;

    q.push_back({x0, y0, 0});
    visited[y0][x0] = '~';

    while (q.size()) {
        auto [x, y, cost] = q.front();
        q.pop_front();

        if (DEBUG) { cout << x << "," << y << ":" << cost << endl; }

        if (x == x1 && y == y1) {
            break;
        }

        static const int D[] = { 1, 0, -1, 0, 1 };
        for (auto dir = 0; dir < 4; ++dir) {
            
            auto nx {x + D[dir]}, ny { y + D[dir + 1]};

            if (a[ny][nx] == '.' && visited[ny][nx] != '~') {
                if (visited[ny][nx] < char(cnt)) {

                    if (nx == x1 && ny == y1) {
                        if (DEBUG) { cout << "Final:" << cost + 1 << endl; }
                        return cost + 1;
                    }

                    if (!use_portals) {
                        continue;
                    }


                    string portal = name[visited[ny][nx]];
                    if (DEBUG) { cout << portal << ':' << cost + 1 << endl; }
                    visited[ny][nx] = '~';

                    auto portal2 = other(portal);

                    auto [ px, py ] = s2p[portal2];
                    visited[py][px] = '~';
                    q.push_back({ px , py, cost + 2 });


                    continue;
                }
                if (DEBUG) { cout << x << ',' << y << '>' << nx << ',' << ny << ':' << cost + 1 << endl; }
                q.push_back({ nx, ny, cost + 1 });
                visited[ny][nx] = '~';
            }
        }


    }
    return 0;

}


int part1()
{
    auto a = read("input20.txt");

    VS visited;
    VS name;

    for (const auto& s : a) {
        cout << s << '\n';
        string scopy(s);
        replace(scopy.begin(), scopy.end(), '.', char(125));
        visited.push_back(scopy);
        //cout << scopy << endl;
    }

    MSPI s2p = preprocess(a);

    cout << s2p.size() << endl;

    int cnt = 0;
    for (const auto& sp : s2p) {
        const auto& [ s, pos ] = sp;
        cout << s << ":" << pos.first << "," << pos.second << endl;
        visited[pos.second][pos.first] = char(cnt++);
        name.push_back(s);
    }

    auto posaa = s2p["AA"];
    auto [x0, y0] = posaa;
    auto poszz = s2p["ZZ"];
    auto [x1, y1] = poszz;

    struct Node { int x{0}; int y{0}; int cost{0}; };

    deque<Node> q;

    q.push_back({x0, y0, 0});
    visited[y0][x0] = '~';

    while (q.size()) {
        auto [x, y, cost] = q.front();
        q.pop_front();

        if (DEBUG) { cout << x << "," << y << ":" << cost << endl; }

        if (x == x1 && y == y1) {
            break;
        }

        static const int D[] = { 1, 0, -1, 0, 1 };
        for (auto dir = 0; dir < 4; ++dir) {
            
            auto nx {x + D[dir]}, ny { y + D[dir + 1]};

            if (a[ny][nx] == '.' && visited[ny][nx] != '~') {
                if (visited[ny][nx] < char(cnt)) {

                    if (nx == x1 && ny == y1) {
                        cout << "Final:" << cost + 1 << endl;
                        return cost + 1;
                    }



                    string portal = name[visited[ny][nx]];
                    if (DEBUG) { cout << portal << ':' << cost + 1 << endl; }
                    visited[ny][nx] = '~';

                    auto portal2 = other(portal);

                    auto [ px, py ] = s2p[portal2];
                    visited[py][px] = '~';
                    q.push_back({ px , py, cost + 2 });


                    continue;
                }
                if (DEBUG) { cout << x << ',' << y << '>' << nx << ',' << ny << ':' << cost + 1 << endl; }
                q.push_back({ nx, ny, cost + 1 });
                visited[ny][nx] = '~';
            }
        }


    }
    return 0;
} 


int main(int, char**)
{
    //part1();

    
    auto a = read("input20.txt");
    MSPI s2p = preprocess(a);
    int d = distance(a, s2p, "AA", "ZZ");

    cout << d << endl;

    cout << "Solution part 1: " << distance(a, s2p, "AA", "ZZ", true) << endl; 

    map<pair<string, string>, int> dmap;

    for (auto it = s2p.begin(); it != s2p.end(); ++it) {
        auto it2 { it };
        ++it2;
        for (; it2 != s2p.end(); ++it2) {
            int d = distance(a, s2p, it->first, it2->first);

            if (d > 0) {
                cout << it->first << " > " << it2->first << " : " << d << endl;
                dmap[pair{it->first, it2->first}] = d;
                dmap[pair{it2->first, it->first}] = d;

            }
        }
    }

    using State = pair<string, int>;
    using Cost2State = multimap<int, State>;

    Cost2State pq;

    pq.insert({0, pair{"AA", 0}}); 

    while (!pq.empty()) {
        
        auto [ cost, pr ] = *pq.begin();
        auto [ last_pos, last_level ] = pr;

        pq.erase(pq.begin());
            
        if (DEBUG) { cout << cost << "," << last_level << "," << last_pos << endl; }

        if (last_pos == "ZZ" && last_level == 0) {
            cout << "Solution part 2:" << cost << endl;
            break;
        }

        for (auto it = dmap.begin(); it != dmap.end(); ++it) {
            const auto& [ pr, dist ] = *it;

            const auto& [from, to ] = pr;

            //if (from[0] != last_pos[0] || from[1] != last_pos[1] ) { 
            if (from != last_pos) {
                continue;
            }

            if (to == "AA") { 
                continue;
            }
            if (to == "ZZ") {
                if (last_level == 0) {
                    pq.insert({dist + cost, pair{to, last_level}}); 
                }
            }

            auto to_portal = other(to);
            if (to.size() == 2) {
                if (last_level == 0) {
                    continue;
                }
                pq.insert({dist + cost + 1, pair{to_portal, last_level - 1}}); 
            } else {
                pq.insert({dist + cost + 1, pair{to_portal, last_level + 1}}); 
            }
        }

    }

    return 0; 

}

