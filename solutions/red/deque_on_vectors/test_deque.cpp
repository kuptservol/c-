#include "test_runner.h"
#include "deque.cpp"
#include <deque>

void TestDeque() {
    deque<int> d;
    d.push_back(2);
    d.push_back(1);
    d.push_front(3);
    d.push_front(4);

    cout << d[0];
    cout << d[1];
    cout << d[2];
    cout << d[3] << endl;

    cout << d.front();
    cout << d.back();

    Deque<int> deq;
    ASSERT_EQUAL(deq.Empty(), true)
    ASSERT_EQUAL(deq.Size(), 0)

    deq.PushBack(2);
    deq.PushBack(1);
    ASSERT_EQUAL(deq[0], 2)
    ASSERT_EQUAL(deq[1], 1)

    deq.PushFront(3);
    deq.PushFront(4);

    ASSERT_EQUAL(deq[0], 4)
    ASSERT_EQUAL(deq[1], 3)
    ASSERT_EQUAL(deq[2], 2)
    ASSERT_EQUAL(deq[3], 1)

    deq[2] = 50;
    ASSERT_EQUAL(deq[2], 50)

    ASSERT_EQUAL(deq.At(2), 50)

    deq.At(2) = 100;
    ASSERT_EQUAL(deq.At(2), 100)

    try {

        ASSERT_EQUAL(deq.At(100), 100)
    } catch (out_of_range ex) {

    }


    ASSERT_EQUAL(deq.Front(), 4)
    ASSERT_EQUAL(deq.Back(), 1)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestDeque);
    return 0;
}