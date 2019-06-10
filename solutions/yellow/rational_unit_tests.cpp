#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};
//
//class Rational {
//public:
//
//    Rational();
//    Rational(int numerator, int denominator) {
//    }
//
//    int Numerator() const {
//    }
//
//    int Denominator() const {
//    }
//};

void DefaultConstructor() {
    Rational rational;

    AssertEqual(rational.Numerator(), 0);
    AssertEqual(rational.Denominator(), 1);
}

void ConstructorReduce() {
    Rational rational(4, 6);

    AssertEqual(rational.Numerator(), 2);
    AssertEqual(rational.Denominator(), 3);

    Rational rational2(3, 9);

    AssertEqual(rational2.Numerator(), 1);
    AssertEqual(rational2.Denominator(), 3);
}

void ConstructorReduce2() {
    Rational rational(10, 2);

    AssertEqual(rational.Numerator(), 5);
    AssertEqual(rational.Denominator(), 1);
}

void ConstructorSimple() {
    Rational rational(2, 3);

    AssertEqual(rational.Numerator(), 2);
    AssertEqual(rational.Denominator(), 3);
}

void ConstructorMinus() {
    Rational rational(-2, 3);

    AssertEqual(rational.Numerator(), -2);
    AssertEqual(rational.Denominator(), 3);

    Rational rational2(2, -3);

    AssertEqual(rational2.Numerator(), -2);
    AssertEqual(rational2.Denominator(), 3);

    Rational rational3(-2, -3);

    AssertEqual(rational3.Numerator(), 2);
    AssertEqual(rational3.Denominator(), 3);
}

void NullNumerator() {
    Rational rational(0, 3);

    AssertEqual(rational.Numerator(), 0);
    AssertEqual(rational.Denominator(), 1);
}


int main() {
    TestRunner runner;
    runner.RunTest(DefaultConstructor, "DefaultConstructor");
    runner.RunTest(ConstructorReduce, "ConstructorReduce");
    runner.RunTest(ConstructorSimple, "ConstructorSimple");
    runner.RunTest(ConstructorReduce2, "ConstructorReduce2");
    runner.RunTest(ConstructorMinus, "ConstructorMinus");
    runner.RunTest(NullNumerator, "NullNumerator");
    return 0;
}