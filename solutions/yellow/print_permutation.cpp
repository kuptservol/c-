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

int main() {

    int num = 3;
    cin >> num;

    vector<int> vec(num);

    vector<vector<int>> perm;

    iota(begin(vec), end(vec), 1);

    perm.push_back(vec);
    while (next_permutation(begin(vec), end(vec))) {
        perm.push_back(vec);
    }

    sort(rbegin(perm), rend(perm));
    for (auto it = begin(perm); it != end(perm); it++) {
        vector<int> next = *it;
        Print(begin(next), end(next));
    }

    return 0;
}
