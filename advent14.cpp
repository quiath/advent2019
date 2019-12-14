#include <iostream>
#include <ios>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

using I = long long;
using VI = vector<long long>;
using VVI = vector<vector<long long>>;

int parse_with_encode(const string& filename, VVI& v)
{
    ifstream f(filename, ios::in);

    map<string, long> s2i;

    s2i["FUEL"] = 1;

    s2i["ORE"] = 0;

    enum State { START = 0, LNUM, LNAME, ARROW, RNUM, RNAME = START };

    VI c; 

    State state { START };

    while (f)
    {
        string s;
        f >> s;
#ifdef DEBUG
        cout << s << endl;
#endif
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
#ifdef DEBUG    
    for (auto it = s2i.begin(); it != s2i.end(); ++it)
    {
        auto [k, v] = *it;
        cout << k << ":" << v << endl; 
    }

    std::cout << s2i.size() << endl;
#endif
    int n =  s2i.size();

    return n;
}

VVI get_rules_from_input(const VVI& v, int n)
{
    VVI tgt(n);
    for (const auto& c : v)
    {
        VI t(n, 0);
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
    return tgt;
}


I find(const VVI& rules, I target_fuel)
{
    auto fit = find_if(rules.begin(), rules.end(), [](const VI& v){ return v.size() > 2 && v[1] != 0; });

    assert(fit != rules.end());

    VI v(*fit);

    for (auto& x : v)
    {
        x *= target_fuel;
    }

    int nr = rules.size();
    int n = rules[1].size();
    assert(n == nr);

    bool ok = true; 
    do 
    {

        ok = true;
        for (int i = 1; i < n; ++i)
        {
            if (v[i] > 0)
            {
                const auto& w { rules[i] };

                auto to_reduce { v[i] };
                auto reduce_by { -rules[i][i] };
                
                auto mul = to_reduce / reduce_by + (to_reduce % reduce_by > 0);

                for (int j = 0; j < n; ++j)
                {
                   v[j] += mul * w[j]; 
                }
                ok = false;
                break;
            }
        }

    }
    while (!ok);

#ifdef DEBUG
    for (auto k : v)
    {
        cout << k << " ";
    }
    cout << "\nOre: " << v[0] << " for fuel " << target_fuel << endl;
#endif

    return v[0];

}

int main()
{
    VVI v;
    auto n = parse_with_encode("input14.txt", v);
    VVI rules = get_rules_from_input(v, n);

    I ore = find(rules, 1LL);

    cout << "One fuel unit needs ore: " << ore << endl;

    // part 2
    I target_ore = 1000'000'000'000LL;
    
    // upper bound
    I fuel { 1LL };
    I need_ore { 0LL };
    I prev_fuel { 0LL };
    do 
    {
        prev_fuel = fuel;
        fuel *= 2;
        need_ore = find(rules, fuel);
    }
    while (need_ore <= target_ore);

    
    // lower bound
    I left {prev_fuel}, right{fuel};
    while (left < right) 
    {
        I mid = (left + right) / 2;
        I res = find(rules, mid);
        if (res < target_ore)
        {
            left = mid+ 1;
        }
        else if (res >= target_ore)
        {
            right = mid;
        }

    }
    I result = (find(rules, left) > target_ore) ?  left - 1 : left;
    cout << "Given ore: " << target_ore << " max fuel is " << result << endl;
    

    return 0;
}
