#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <exception>
#include <string>

class Matrix {
public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, double value);
    Matrix(const std::vector<std::vector<double>>& v);
    Matrix(const Matrix& other);
    ~Matrix();

    Matrix operator+(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
    Matrix& operator=(const Matrix& other);

    bool at(int i, int j) const;
    int getRows() const;
    int getCols() const;
    bool isSameDimension(const Matrix& other) const;
    bool canMultiply(const Matrix& other) const;

private:
    int row;
    int col;
    double** arr;
};

class MatrixException : public std::exception {
private:
    std::string error_msg;
public:
    MatrixException(const std::string& msg) : error_msg(msg) {}
    const char* what() const noexcept override { return error_msg.c_str(); }
};

#endif // MATRIX_H    