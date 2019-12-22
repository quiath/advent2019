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

int main(int, char**)
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
    cout << distance(a.begin(), it) << endl;

    return 0;
}
