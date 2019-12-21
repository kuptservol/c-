#include "../../test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
    T *Allocate() {
        if (released.empty()) {
            T *new_allocated = new T;
            allocated.insert(new_allocated);

            return new_allocated;
        } else {
            T *from_release_to_allocated = released.front();
            allocated.insert(from_release_to_allocated);
            released.pop();

            return from_release_to_allocated;
        }
    }

    T *TryAllocate() {
        if (released.empty()) {
            return nullptr;
        } else {
            T *from_release_to_allocated = released.front();
            released.pop();
            allocated.insert(from_release_to_allocated);

            return from_release_to_allocated;
        }
    }

    void Deallocate(T *object) {
        if (allocated.count(object) == 0) {
            throw invalid_argument("");
        }

        allocated.erase(object);
        released.push(object);
    }

    ~ObjectPool() {
        for (auto it = begin(allocated); it != end(allocated); it++) {
            delete *it;
        }

        while (!released.empty()) {
            auto element = released.front();
            delete element;
            released.pop();
        }
    }

private:
    set<T *> allocated;
    queue<T *> released;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    bool double_deallocate_fails = false;
    try {
        pool.Deallocate(p2);
    } catch (exception &ex) {
        double_deallocate_fails = true;
    }
    ASSERT_EQUAL(double_deallocate_fails, true);

    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}