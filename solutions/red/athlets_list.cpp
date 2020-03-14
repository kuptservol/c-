#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main() {
    const int MAX_ATHLETES = 100'000;

    int n_athlets;
    cin >> n_athlets;

    list<int> athlets_row;
    using Position = list<int>::iterator;
    vector<Position> athlete_pos(MAX_ATHLETES + 1, athlets_row.end());

    for (int i = 1; i <= n_athlets; i++) {
        int athlet, next_athlet;

        cin >> athlet >> next_athlet;

        athlete_pos[athlet] = athlets_row.insert(
                athlete_pos[next_athlet],
                athlet
        );
    }

    for (int x : athlets_row) {
        cout << x << '\n';
    }
    return 0;
}