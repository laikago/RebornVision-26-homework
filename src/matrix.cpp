#include "matrix.h"

Matrix::Matrix() : row(0), col(0), arr(nullptr) {}

Matrix::Matrix(int rows, int cols) : row(rows), col(cols) {
    arr = new double*[row];
    for (int i = 0; i < row; ++i) {
        arr[i] = new double[col]();
    }
}

Matrix::Matrix(int rows, int cols, double value) : row(rows), col(cols) {
    arr = new double*[row];
    for (int i = 0; i < row; ++i) {
        arr[i] = new double[col];
        for (int j = 0; j < col; ++j) {
            arr[i][j] = value;
        }
    }
}

Matrix::Matrix(const std::vector<std::vector<double>>& v) {
    if (v.empty()) {
        row = 0;
        col = 0;
        arr = nullptr;
        return;
    }
    row = v.size();
    col = v[0].size();
    arr = new double*[row];
    for (int i = 0; i < row; ++i) {
        arr[i] = new double[col];
        for (int j = 0; j < col; ++j) {
            arr[i][j] = v[i][j];
        }
    }
}

Matrix::Matrix(const Matrix& other) : row(other.row), col(other.col) {
    arr = new double*[row];
    for (int i = 0; i < row; ++i) {
        arr[i] = new double[col];
        for (int j = 0; j < col; ++j) {
            arr[i][j] = other.arr[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < row; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (!isSameDimension(other)) {
        throw MatrixException("Matrix dimensions must agree for addition.");
    }
    Matrix result(row, col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            result.arr[i][j] = arr[i][j] + other.arr[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (!canMultiply(other)) {
        throw MatrixException("Number of columns in first matrix must be equal to number of rows in second matrix for multiplication.");
    }
    Matrix result(row, other.col);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < other.col; ++j) {
            double sum = 0.0;
            for (int k = 0; k < col; ++k) {
                sum += arr[i][k] * other.arr[k][j];
            }
            result.arr[i][j] = sum;
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    os << "Matrix " << m.row << "x" << m.col << ":\n";
    for (int i = 0; i < m.row; ++i) {
        os << "[ ";
        for (int j = 0; j < m.col; ++j) {
            os.width(6);
            os.precision(2);
            os << std::fixed << m.arr[i][j] << " ";
        }
        os << "]\n";
    }
    return os;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        for (int i = 0; i < row; ++i) {
            delete[] arr[i];
        }
        delete[] arr;

        row = other.row;
        col = other.col;
        arr = new double*[row];
        for (int i = 0; i < row; ++i) {
            arr[i] = new double[col];
            for (int j = 0; j < col; ++j) {
                arr[i][j] = other.arr[i][j];
            }
        }
    }
    return *this;
}

bool Matrix::at(int i, int j) const {
    return i >= 0 && i < row && j >= 0 && j < col;
}

int Matrix::getRows() const {
    return row;
}

int Matrix::getCols() const {
    return col;
}

bool Matrix::isSameDimension(const Matrix& other) const {
    return row == other.row && col == other.col;
}

bool Matrix::canMultiply(const Matrix& other) const {
    return col == other.row;
}    
