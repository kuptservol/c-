#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;

struct Key {
    int row;
    int column;
};

bool operator<(const Key &one, const Key &two) {
    if (one.row == two.row) {
        return one.column < two.column;
    } else {
        return one.row < two.row;
    }
}

class Matrix {

public :
    Matrix() : Matrix(0, 0) {}

    Matrix(int num_rows, int num_columns) {
        n_rows = num_rows;
        n_columns = num_columns;
        if (n_rows < 0 || num_columns < 0) {
            throw out_of_range("");
        }

        if (n_rows == 0 || num_columns == 0) {
            empty = true;
        }
    }

    void Reset(int num_rows, int num_columns) {
        for (int i = 0; i < GetNumRows(); i++) {
            for (int j = 0; j < GetNumColumns(); j++) {
                At(i, j) = 0;
            }
        }

        n_rows = num_rows;
        n_columns = num_columns;
        if (n_rows < 0 || num_columns < 0) {
            throw out_of_range("");
        }

        if (n_rows == 0 || num_columns == 0) {
            empty = true;
        }
    }

    int At(int row, int column) const {
        if (row < 0 || row >= n_rows || column < 0 || column >= n_columns) {
            throw out_of_range("");
        }
        if (matrix.count({row, column}) == 0) {
            return 0;
        } else {
            return matrix.at({row, column});
        }
    }

    int &At(int row, int column) {
        if (row < 0 || row >= n_rows || column < 0 || column >= n_columns) {
            throw out_of_range("");
        }
        return matrix[{row, column}];
    }

    int GetNumRows() const {
        return n_rows;
    }

    int GetNumColumns() const {
        return n_columns;
    }

    bool Empty() const {
        return empty;
    }

private:
    int n_rows;
    int n_columns;
    bool empty = false;

    map<Key, int> matrix;
};

Matrix operator+(Matrix one, Matrix two) {
    if (one.GetNumColumns() != two.GetNumColumns() || one.GetNumRows() != two.GetNumRows()) {
        throw invalid_argument("");
    }

    if (one.Empty() && two.Empty()) {
        return one;
    }

    Matrix sum(one.GetNumRows(), one.GetNumColumns());

    for (int i = 0; i < one.GetNumRows(); i++) {
        for (int j = 0; j < one.GetNumColumns(); j++) {
            sum.At(i, j) = one.At(i, j) + two.At(i, j);
        }
    }

    return sum;
}

bool operator==(Matrix one, Matrix two) {
    if (one.Empty() && two.Empty()) {
        return true;
    }

    if (one.GetNumColumns() != two.GetNumColumns() || one.GetNumRows() != two.GetNumRows()) {
        return false;
    }

    Matrix sum(one.GetNumRows(), one.GetNumColumns());

    for (int i = 0; i < one.GetNumRows(); i++) {
        for (int j = 0; j < one.GetNumColumns(); j++) {
            if (one.At(i, j) != two.At(i, j)) {
                return false;
            }
        }
    }

    return true;
}

istream &operator>>(istream &stream, Matrix &matrix) {
    int rows;
    int columns;

    stream >> rows;
    stream >> columns;

    matrix.Reset(rows, columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int val;
            stream >> val;
            matrix.At(i, j) = val;
        }
    }

    return stream;
}

ostream &operator<<(ostream &stream, const Matrix &matrix) {
    stream << matrix.GetNumRows() << " " << matrix.GetNumColumns() << endl;

    for (int i = 0; i < matrix.GetNumRows(); i++) {
        for (int j = 0; j < matrix.GetNumColumns(); j++) {
            stream << matrix.At(i, j) << " ";
        }

        stream << endl;
    }

    return stream;
}

int main() {
    Matrix one;
    Matrix two;

    cin >> one >> two;

    cout << one + two << endl;

    return 0;
}