#include "test_runner.h"

#include <vector>

using namespace std;

template<typename T>
class LinkedList {
public:
    struct Node {
        T value;
        Node *next = nullptr;
    };

    ~LinkedList() {
        Node *next = head;
        while (next != nullptr) {
            auto prev = next;
            next = next->next;
            delete prev;
        }
    }

    void PushFront(const T &value) {
        Node *new_node = new Node;
        new_node->value = value;
        if (head != nullptr) {
            new_node->next = head;
        }

        head = new_node;
    }

    void InsertAfter(Node *node, const T &value) {
        if (node == nullptr) {
            PushFront(value);
            return;
        }

        Node *new_node = new Node;
        new_node->value = value;

        auto next_after_current = node->next;
        new_node->next = next_after_current;
        node->next = new_node;
    }

    void RemoveAfter(Node *node) {
        if (node == nullptr) {
            PopFront();
            return;
        }
        auto next_to_delete = node->next;
        if (next_to_delete == nullptr) {
            return;
        }

        auto new_next = next_to_delete->next;
        node->next = new_next;

        delete next_to_delete;
    }

    void PopFront() {
        if (head == nullptr) {
            return;
        }

        Node *old_head = head;
        head = head->next;

        delete old_head;
    }

    Node *GetHead() { return head; }

    const Node *GetHead() const { return head; }

private:
    Node *head = nullptr;
};

template<typename T>
vector<T> ToVector(const LinkedList<T> &list) {
    vector<T> result;
    for (auto node = list.GetHead(); node; node = node->next) {
        result.push_back(node->value);
    }
    return result;
}

void TestPushFront() {
    LinkedList<int> list;

    list.PushFront(1);
    ASSERT_EQUAL(list.GetHead()->value, 1);
    list.PushFront(2);
    ASSERT_EQUAL(list.GetHead()->value, 2);
    list.PushFront(3);
    ASSERT_EQUAL(list.GetHead()->value, 3);

    const vector<int> expected = {3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);
}

void TestInsertAfter() {
    LinkedList<string> list;

    list.PushFront("a");
    auto head = list.GetHead();
//    ASSERT(head);
    ASSERT_EQUAL(head->value, "a");

    list.InsertAfter(head, "b");
    const vector<string> expected1 = {"a", "b"};
    ASSERT_EQUAL(ToVector(list), expected1);

    list.InsertAfter(head, "c");
    const vector<string> expected2 = {"a", "c", "b"};
    ASSERT_EQUAL(ToVector(list), expected2);

    LinkedList<string> list2;
    list2.InsertAfter(nullptr, "1");
    list2.InsertAfter(nullptr, "1");
    list2.InsertAfter(nullptr, "1");
}

void TestRemoveAfter() {
    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.PushFront(i);
    }

    const vector<int> expected = {5, 4, 3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);

    auto next_to_head = list.GetHead()->next;
    list.RemoveAfter(next_to_head); // ÑƒÐ´Ð°Ð»ÑÐµÐ¼ 3
    list.RemoveAfter(next_to_head); // ÑƒÐ´Ð°Ð»ÑÐµÐ¼ 2

    const vector<int> expected1 = {5, 4, 1};
    ASSERT_EQUAL(ToVector(list), expected1);

    while (list.GetHead()->next) {
        list.RemoveAfter(list.GetHead());
    }
    ASSERT_EQUAL(list.GetHead()->value, 5);

    LinkedList<string> list2;
    list2.RemoveAfter(nullptr);
}

void TestPopFront() {
    LinkedList<int> list;

    for (int i = 1; i <= 5; ++i) {
        list.PushFront(i);
    }
    for (int i = 1; i <= 5; ++i) {
        list.PopFront();
    }
    ASSERT(list.GetHead() == nullptr);

    LinkedList<string> list2;
    list2.PopFront();
    list2.PopFront();
    list2.PopFront();
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestPushFront);
    RUN_TEST(tr, TestInsertAfter);
    RUN_TEST(tr, TestRemoveAfter);
    RUN_TEST(tr, TestPopFront);
    return 0;
}