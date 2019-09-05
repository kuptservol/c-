#include "test_runner.h"
#include <vector>

using namespace std;

template<typename T>
class Table {
public:
    Table(const size_t rows_, const size_t columns_) : rows(rows_), columns(columns_) {
        vector<vector<T>> table_(rows);

        for (int i = 0; i < rows; i++) {
            table_[i] = vector<T>(columns_, T());
        }

        table = table_;
    }

    void Resize(const size_t rows_, const size_t columns_) {
        rows = rows_;
        columns = columns_;
        table.resize(rows_);
        for (auto &row : table) {
            row.resize(columns_, T());
        }
    }

    pair<size_t, size_t> Size() const {
        return make_pair(rows, columns);
    }

    vector<T> &operator[](size_t index) {
        return table[index];
    }

    const vector<T> &operator[](size_t index) const {
        return table[index];
    }

private:
    size_t rows;
    size_t columns;
    vector<vector<T>> table;
};

void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
    ASSERT_EQUAL(t[0][0], 42);
    ASSERT_EQUAL(t[0][1], 0);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}