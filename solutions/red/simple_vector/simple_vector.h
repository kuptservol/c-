#pragma once

#include <cstdlib>

template<typename T>
class SimpleVector {
public:
    SimpleVector() {
        buffer = nullptr;
        size = 0;
        capacity = 0;
    }

    explicit SimpleVector(size_t size_) {
        size = size_;
        capacity = size;
        buffer = new T[capacity];
    }

    ~SimpleVector() {
        delete[] buffer;
    }

    T &operator[](size_t index) {
        return *(buffer + index);
    }

    T *begin() {
        return buffer;
    }

    T *end() {
        return buffer + size;
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return capacity;
    }

    void PushBack(const T &value) {
        if (Size() == Capacity()) {
            size_t new_capacity;
            if (Capacity() == 0) {
                new_capacity = 1;
            } else {
                new_capacity = Capacity() * 2;
            }

            T *new_buffer = new T[new_capacity];
            for (int i = 0; i < size; ++i) {
                new_buffer[i] = buffer[i];
            }

            delete[] buffer;
            buffer = new_buffer;
            capacity = new_capacity;
        }

        buffer[size++] = value;
    }

private:
    T *buffer;
    size_t capacity;
    size_t size;
};