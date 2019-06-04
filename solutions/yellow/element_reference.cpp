#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

template<typename K, typename V>
V &GetRefStrict(map<K, V> &m, const K &key) {
    if (m.count(key) > 0) {
        return m[key];
    } else {
        throw runtime_error("");
    }
}

int main() {

    map<int, string> m = {{0, "value"}};
    string &item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue

    map<int, int> m2 = {{0, 2}};
    int &item2 = GetRefStrict(m2, 0);
    item2 = 3;
    cout << m2[0] << endl; // выведет newvalue

    return 0;
}
