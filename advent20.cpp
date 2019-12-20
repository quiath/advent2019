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
    }


    int cnt = 0;
    for (const auto& sp : s2p) {
        const auto& [ s, pos ] = sp;
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




int main(int, char**)
{
    
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

