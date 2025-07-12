#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <stdexcept>

// 自定义异常类
class MatrixException : public std::exception {
private:
    std::string message;
public:
    MatrixException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Matrix {
private:
    std::vector<std::vector<double>> data;
    int rows;  // 行数
    int cols;  // 列数

public:
    // 构造函数系列
    Matrix();  // 默认构造函数
    Matrix(int rows, int cols);  // 维度构造函数
    Matrix(int rows, int cols, double value);  // 初始值构造函数
    Matrix(const std::vector<std::vector<double>>& vec);  // 向量构造函数
    Matrix(const Matrix& other);  // 拷贝构造函数

    // 运算符重载
    Matrix operator+(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix& operator=(const Matrix& other);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);

    // 安全访问与维度查询
    double& at(int row, int col);
    const double& at(int row, int col) const;
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // 工具函数
    bool isSameDimension(const Matrix& other) const;
    bool canMultiply(const Matrix& other) const;
};

#endif // MATRIX_H
