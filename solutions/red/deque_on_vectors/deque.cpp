#include <vector>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class Deque {
public:
    Deque() {}

    const bool Empty() const {
        return left.empty() && right.empty();
    }

    const int Size() const {
        return left.size() + right.size();
    }

    T &operator[](size_t index) {
        return GetAt(index);
    }

    const T &operator[](size_t index) const {
        return GetAt(index);
    }

    T &Front() {
        return GetAt(0);
    }

    const T &Front() const {
        return GetAt(0);
    }

    T &Back() {
        return GetAt(Size() - 1);
    }

    const T &Back() const {
        return GetAt(Size() - 1);
    }

    T &At(size_t index) {
        if (index >= Size()) {
            throw out_of_range("");
        }

        return GetAt(index);
    }

    const T &At(size_t index) const {
        if (index >= Size()) {
            throw out_of_range("");
        }

        return GetAt(index);
    }

    T &GetAt(size_t index) {
        if (index < right.size()) {
            return right[right.size() - 1 - index];
        } else {
            return left[index - right.size()];
        }
    }

    const T &GetAt(size_t index) const {
        if (index < right.size()) {
            return right[right.size() - 1 - index];
        } else {
            return left[index - right.size()];
        }
    }

    void PushFront(const T &val) {
        right.push_back(val);
    }

    void PushBack(const T &val) {
        left.push_back(val);
    }

private:
    vector<T> left;
    vector<T> right;
};
