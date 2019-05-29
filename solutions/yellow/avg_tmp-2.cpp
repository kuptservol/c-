#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;


int main() {
    int count;
    cin >> count;

    int64_t sum = 0;
    vector<int> tmps(count);

    for (int &tmp : tmps) {
        cin >> tmp;
        sum += tmp;
    }

    int avg = sum / static_cast<int64_t>(tmps.size());

    vector<int> tmpsOverAvg;
    for (int i = 0; i < tmps.size(); i++) {
        if (tmps[i] > avg) tmpsOverAvg.push_back(i);
    }

    cout << tmpsOverAvg.size() << endl;

    for (int i : tmpsOverAvg) {
        cout << i << " ";
    }

    return 0;
}