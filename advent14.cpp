#include <iostream>
#include <ios>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

using VI = vector<int>;
using VVI = vector<vector<int>>;

//int find(const VVI& tgt, const VVI& src)
int find(const VVI& rules)

{
    auto fit = find_if(rules.begin(), rules.end(), [](const VI& v){ return v.size() > 2 && v[1] != 0; });

    if (fit == rules.end())
    {
        cout << " ERR " << endl;
    }

    VI v(*fit);

    int nr = rules.size();
    int n = rules[1].size();

    bool ok = true; 
    do 
    {
        for (auto k : v)
        {
            cout << k << " ";
        }
        cout << endl;


        ok = true;
        for (int i = 1; i < n; ++i)
        {
            if (v[i] > 0)
            {
                const auto& w { rules[i] };
                for (int j = 0; j < n; ++j)
                {
                   v[j] += w[j]; 
                }
                ok = false;
                break;
            }
        }

    }
    while (!ok);

    for (auto k : v)
    {
        cout << k << " ";
    }
    cout << endl;

}

int main()
{

    //ifstream f("input14a.txt", ios::in);
    ifstream f("input14.txt", ios::in);

    map<string, long> s2i;
    //map<string, long> i2s;

    s2i["FUEL"] = 1;

    s2i["ORE"] = 0;

    enum State { START = 0, LNUM, LNAME, ARROW, RNUM, RNAME = START };

    vector<vector<int>> v;

    vector<int> c; 

    State state { START };

    while (f)
    {
        string s;
        f >> s;
        cout << s << endl;

        if (s.length() == 0)
        {
            continue;
        }

        switch (state)
        {
            case State::LNUM:
            case State::RNUM:
                {
                    if (s[s.length() - 1] == ',')
                    {
                        s.resize(s.length() - 1);
                    }
                    int k { 0 };
                    if (auto p = s2i.find(s); p != s2i.end())
                    {
                        k = p->second;
                    } 
                    else
                    {
                        k = s2i.size();
                        s2i[s] = k;
                    }
                    c.push_back(k);
                    if (state == State::RNUM)
                    {
                        v.push_back(c);
                        //cout << c[c.size() - 2] << "," << c[c.size() - 1] << "," << s << endl;
                        c.clear();
                        state = State::RNAME;
                    }
                    else
                    {

                        state = State::LNAME;
                    }
                    break;
               } 
            case State::LNAME:
                if (s == "=>")
                {
                    state = State::ARROW;
                    break;
                }
                [[fallthrough]];
            case State::START:
                c.push_back(stoi(s));
                state = State::LNUM;
                break;

            case State::ARROW:
                c.push_back(stoi(s));
                state = State::RNUM;
                break;
        }
    }
    
    for (auto it = s2i.begin(); it != s2i.end(); ++it)
    {
        auto [k, v] = *it;
        cout << k << ":" << v << endl; 
    }

    std::cout << s2i.size() << endl;

    int n =  s2i.size();

    vector<vector<int>> tgt(n);
    vector<vector<int>> src;
#if 0
    for (const auto& c : v)
    {
        vector t(n, 0);
        t[c[c.size() - 1]] = c[c.size() - 2];
        tgt.push_back(t);


        for (auto k : t)
        {
            cout << k << " ";
        }
        cout << " < " ;

        fill(t.begin(), t.end(), 0);
        for (int i = 0; i < c.size() - 2; i += 2)
        {
            t[c[i + 1]] = c[i];
        }
        
        for (auto k : t)
        {
            cout << k << " ";
        }
        cout << endl;

        src.push_back(std::move(t));
    }
#endif
    for (const auto& c : v)
    {
        vector t(n, 0);
        auto tgtid = c[c.size() - 1];
        t[tgtid] = -c[c.size() - 2];
 
        for (int i = 0; i < c.size() - 2; i += 2)
        {
            t[c[i + 1]] = c[i];
        }
        
        for (auto k : t)
        {
            cout << k << " ";
        }
        cout << endl;

        tgt[tgtid] = std::move(t);
    }
    for (int i = 0; i < tgt.size(); ++i) 
    {   
        cout << i << ": ";
        for (auto k : tgt[i])
        {
            cout << k << " ";
        }
        cout << endl;

    }

    find(tgt);


    return 0;
}
