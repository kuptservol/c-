#include "../../../test_runner.h"
#include "learner.cpp"

#include <string>
#include <vector>

using namespace std;

int main() {
    Learner learner;
    string line;
    int i = 2;
    while (getline(cin, line) && i >= 0) {
        vector<string> words;
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        cout << learner.Learn(words) << "\n";
        i--;
    }
    cout << "=== known words ===\n";
    for (auto word : learner.KnownWords()) {
        cout << word << "\n";
    }
}