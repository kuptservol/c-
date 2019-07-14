#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include <numeric>
#include <deque>

using namespace std;

class Animal {
public:
    Animal(const string &name) : Name(name) {

    }

    const string Name;
};


class Dog : public Animal {
public:
    Dog(const string &name) : Animal(name) {

    }

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};


int main() {

    Dog dog("snoop");
    dog.Bark();

    return 0;

}
