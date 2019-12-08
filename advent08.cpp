#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <string_view>
#include <utility>
#include <array>


std::pair<int, int> get_layer_counts(std::string_view sv)
{
    std::array<int, 256> count{};
    for (auto ch : sv)
    {
        ++count[ch]; 
    }
    return { count['0'], count['1'] * count['2'] };
}


int main(int, char**)
{
    using namespace std;
    ifstream inf("input08.txt", ios::in);

    constexpr int sx { 25 } , sy { 6};
    constexpr int lay_len { sx * sy };

    string input;
    inf >> input;

    auto len = input.size();

    cout << len << endl;

    pair best_result { lay_len+ 1, 0 };

    for (auto i = 0U; i < len; i += lay_len)
    {
        string_view layer(&input[i], sx * sy);
        auto curr = get_layer_counts(layer);
        if (curr < best_result) 
        {
            best_result = curr;
        }
    }
    cout << best_result.first << "," << best_result.second << endl;

    string image(lay_len, ' ');

    for (int i = len - lay_len; i >= 0; i -= lay_len)
    {
        string_view layer(&input[i], sx * sy);
        for (auto j = 0U; j < lay_len; ++j)
        {
            switch (layer[j])
            {
                case '0': image[j] = '.'; break;
                case '1': image[j] = 'X'; break;
            }
        }
    }
    for (int i = 0; i < sy; ++i)
    {
        cout << string_view(&image[i * sx], sx) << endl;
    }
    return 0;
}

