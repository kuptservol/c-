#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;


int main() {
    int count;
    uint64_t density;

    cin >> count;
    cin >> density;

    uint64_t weight = 0;
    uint64_t W, H, D;

    for (int i = count; i > 0; i--) {
        cin >> W;
        cin >> H;
        cin >> D;

        weight += density * W * H * D;
    }

    cout << weight;

    //cout << sizeof(int64_t) << " ";
    //cout << sizeof(uint64_t) << endl;

    //cout << numeric_limits <int64_t >:: min () <<" " <<numeric_limits <int64_t >:: max () << endl;
    //cout << numeric_limits <uint64_t >:: min () <<" " <<numeric_limits <uint64_t >:: max () << endl;

    return 0;
}