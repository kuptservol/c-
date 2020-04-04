#pragma once


#include <stdexcept>
#include <array>

using namespace std;

template<typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) : size(a_size) {
        if (a_size > N) {
            throw invalid_argument("");
        }
    }

    T &operator[](size_t index) {
        return array[index];
    }

    const T &operator[](size_t index) const {
        return array[index];
    }

    auto begin() {
        return array.begin();
    }

    auto end() {
        return array.end();
    }

    auto begin() const {
        return array.begin();
    }

    auto end() const {
        return array.end();
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return N;
    }

    void PushBack(const T &value) {
        if (size == N) {
            throw overflow_error("");
        }

        array[size++] = value;
    }

    T PopBack() {
        if (size == 0) {
            throw underflow_error("");
        }

        return array[--size];
    }

private:
    array<T, N> array;
    size_t size;
};
