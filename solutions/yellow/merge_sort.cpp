#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <numeric>
#include "utils.cpp"

using namespace std;

//2 parts
template<typename RandomIt>
void MergeSort2(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 == range_end) {
        return;
    }

    vector<typename RandomIt::value_type> v;
    copy(range_begin, range_end, back_inserter(v));

    auto middle = begin(v) + distance(begin(v), end(v)) / 2;
    MergeSort2(begin(v), middle);
    MergeSort2(middle, end(v));

    merge(begin(v), middle, middle, end(v), range_begin);
}

// 3 parts
template<typename RandomIt>
void MergeSort3(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 == range_end) {
        return;
    }

    vector<typename RandomIt::value_type> v;
    copy(range_begin, range_end, back_inserter(v));

    long dist = distance(begin(v), end(v));
    auto middle1 = begin(v) + dist / 3;
    auto middle2 = begin(v) + dist * 2 / 3;
    MergeSort3(begin(v), middle1);
    MergeSort3(middle1, middle2);
    MergeSort3(middle2, end(v));

    vector<typename RandomIt::value_type> tmp;

    merge(begin(v), middle1, middle1, middle2, back_inserter(tmp));


    merge(begin(tmp), end(tmp), middle2, end(v), range_begin);
}

int main() {
    vector<int> v1 = {6, 4, 7, 6, 4, 4, 0, 1};
    vector<int> v2 = {6, 4, 7, 6, 4, 4, 0, 1, 5};
    MergeSort2(begin(v1), end(v1));
    MergeSort3(begin(v2), end(v2));
    cout << endl;
    return 0;
}
