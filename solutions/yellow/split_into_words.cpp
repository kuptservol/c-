#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

vector<string> SplitIntoWords(const string &s) {
    vector<string> v;

    auto from = begin(s);
    auto to = end(s);

    while (true) {
        auto border = find(from, to, ' ');

        v.push_back({from, border});

        if (border == to) {
            break;
        }

        from = ++border;
    }

    return v;
}


int main() {
    string s = "Ccc Cpp Java Python";

    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;

    return 0;
}
