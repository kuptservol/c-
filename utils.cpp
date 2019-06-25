#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <numeric>

using namespace std;

template<class InputIt>
void Print(InputIt from, InputIt to) {
    for (auto it = from; it != to; it++) {
        cout << *it << " ";
    }

    cout << endl;
}