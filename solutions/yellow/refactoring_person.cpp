#include <utility>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class Person {
public:
    explicit Person(const string &name, const string &type) : Name(name), Type(type) {}

    virtual void Walk(const string &destination) const {
        cout << Type << ": " << Name << " walks to: " << destination << endl;
    }

    string GetName() const {
        return Name;
    }

    string GetType() const {
        return Type;
    }

protected:
    const string Name;
    const string Type;

};

class Student : public Person {
public:

    Student(const string &name, const string &favouriteSong) : Person(name, "Student"), FavouriteSong(favouriteSong) {
    }

    void Learn() const {
        cout << "Student: " << Name << " learns" << endl;
    }

    void Walk(const string &destination) const override {
        LogName();
        cout << " walks to: " << destination << endl;
        LogName();
        cout << " sings a song: " << FavouriteSong << endl;
    }

    void SingSong() const {
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }


private:
    const string FavouriteSong;

    void LogName() const {
        cout << "Student: " << Name;
    }

};


class Teacher : public Person {
public:

    Teacher(const string &name, const string &subject) : Person(name, "Teacher"), Subject(subject) {
    }

    void Teach() const {
        cout << "Teacher: " << Name << " teaches: " << Subject << endl;
    }

private:
    const string Subject;
};


class Policeman : public Person {
public:
    explicit Policeman(const string &name) : Person(name, "Policeman") {
    }

    void Check(const Person &person) {
        cout << "Policeman: " << Name << " checks " << person.GetType() << ". " << person.GetType() << "'s name is: "
             << person.GetName() << endl;

    }
};


void VisitPlaces(const Person &person, const vector<string> &places) {
    for (const auto &p : places) {
        person.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    p.Check(t);
    p.Check(p);
    VisitPlaces(s, {"Moscow", "London"});
    VisitPlaces(p, {"Petrovka, 38"});
    return 0;
}