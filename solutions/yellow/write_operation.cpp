#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <numeric>
#include <deque>

using namespace std;


int main() {
    deque<string> q;

    int start_val;
    cin >> start_val;

    int count;
    cin >> count;

    q.push_back(to_string(start_val));

    string last_operation = "";
    string operation;
    int val;
    while ((count -= 1) >= 0) {
        cin >> operation;
        cin >> val;

        if (last_operation == "+" || last_operation == "-") {
            if (operation == "*" || operation == "/") {
                q.push_front("(");
                q.push_back(")");
            }
        }
        q.push_back(" " + operation + " ");
        q.push_back(to_string(val));

        last_operation = operation;
    }

    for (const auto &x : q) {
        cout << x;
    }

    return 0;

}
