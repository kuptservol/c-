#pragma once

#include <string>
#include <set>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

template<class T>
ostream &operator<<(ostream &os, const set<T> &s);

template<class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m);

template<class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint);

void Assert(bool b, const string &hint);

class TestRunner {
public:
    template<class TestFunc>
    void RunTest(TestFunc func, const string &test_name);

    ~ TestRunner();

private:
    int fail_count = 0;
};

void Assert(bool b, const string &hint) {
    AssertEqual(b, true, hint);
}

TestRunner::~ TestRunner() {
    if (fail_count > 0) {
        cerr << fail_count << " unit tests failed. Terminate" << endl;
        exit(1);
    }
}

template<class T>
ostream &operator<<(ostream &os, const set<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class T>
ostream &operator<<(ostream &os, const vector<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
    os << "{";
    bool first = true;
    for (const auto &kv:m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u << " hint: " << hint;
        throw runtime_error(os.str());
    }
}

#define RUN_TEST(tr, func) \
    tr.RunTest(func , #func)

#define ASSERT_EQUAL(x, y) {                          \
  ostringstream __assert_equal_private_os;            \
  __assert_equal_private_os                           \
    << #x << " != " << #y << ", "                     \
    << __FILE__ << ":" << __LINE__;                   \
  AssertEqual(x, y, __assert_equal_private_os.str()); \
}

#define ASSERT(x) {                                      \
  ostringstream __assert_equal_private_os;               \
  __assert_equal_private_os                              \
    << #x << " != " << " True " << ", "                  \
    << __FILE__ << ":" << __LINE__;                      \
  AssertEqual(x, true, __assert_equal_private_os.str()); \
}

template<class TestFunc>
void TestRunner::RunTest(TestFunc func, const string &test_name) {
    try {
        func();
        cerr << test_name << " OK" << endl;
    } catch (runtime_error &e) {
        ++fail_count;
        cerr << test_name << " fail: " << e.what() << endl;
    }
}