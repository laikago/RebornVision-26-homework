#include "matrix.h"
#include <iostream> 
#include <iomanip>  // 用于格式化输出

// 默认构造函数：创建1x1矩阵，值为0
Matrix::Matrix() : rows(1), cols(1), data({{0.0}}) {}

// 维度构造函数：创建rows x cols矩阵，值为0
Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw MatrixException("维度必须为正整数: " + std::to_string(rows) + "x" + std::to_string(cols));
    }
    data.resize(rows, std::vector<double>(cols, 0.0));
}

// 初始值构造函数：创建rows x cols矩阵，值为value
Matrix::Matrix(int rows, int cols, double value) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw MatrixException("维度必须为正整数: " + std::to_string(rows) + "x" + std::to_string(cols));
    }
    data.resize(rows, std::vector<double>(cols, value));
}

// 向量构造函数：从二维向量初始化矩阵
Matrix::Matrix(const std::vector<std::vector<double>>& vec) {
    if (vec.empty()) {
        throw MatrixException("初始化向量不能为空");
    }
    rows = vec.size();
    cols = vec[0].size();
    // 检查所有行的列数是否一致
    for (const auto& row : vec) {
        if (row.size() != cols) {
            throw MatrixException("向量各行列数不一致");
        }
    }
    data = vec;
}

// 拷贝构造函数
Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), data(other.data) {}

// 赋值运算符重载
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        data = other.data;
    }
    return *this;
}

// 加法运算符重载
Matrix Matrix::operator+(const Matrix& other) const {
    if (!isSameDimension(other)) {
        throw MatrixException("矩阵加法维度不匹配: " + std::to_string(rows) + "x" + std::to_string(cols) 
                            + " + " + std::to_string(other.rows) + "x" + std::to_string(other.cols));
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

// 乘法运算符重载（矩阵乘法）
Matrix Matrix::operator*(const Matrix& other) const {
    if (!canMultiply(other)) {
        throw MatrixException("矩阵乘法不兼容: " + std::to_string(rows) + "x" + std::to_string(cols) 
                            + " * " + std::to_string(other.rows) + "x" + std::to_string(other.cols));
    }
    Matrix result(rows, other.cols, 0.0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

// 输出运算符重载
std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    // 使用英文提示避免编码问题
    os << "Matrix " << mat.rows << "x" << mat.cols << ":\n";
    
    // 保存当前流的格式状态，避免影响后续输出
    std::ios_base::fmtflags originalFlags = os.flags();
    os << std::fixed << std::setprecision(2);  // 设置小数精度
    
    for (int i = 0; i < mat.rows; ++i) {
        os << "[ ";
        for (int j = 0; j < mat.cols; ++j) {
            os << mat.data[i][j] << " ";
        }
        os << "]\n";
    }
    
    // 恢复流的原始格式状态
    os.flags(originalFlags);
    
    return os;
}
// 安全访问（非const版本）
double& Matrix::at(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw MatrixException("访问越界: (" + std::to_string(row) + ", " + std::to_string(col) + ")");
    }
    return data[row][col];
}

// 安全访问（const版本）
const double& Matrix::at(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw MatrixException("访问越界: (" + std::to_string(row) + ", " + std::to_string(col) + ")");
    }
    return data[row][col];
}

// 检查是否与另一个矩阵维度相同
bool Matrix::isSameDimension(const Matrix& other) const {
    return (rows == other.rows) && (cols == other.cols);
}

// 检查是否可以与另一个矩阵相乘（当前矩阵的列数 == 另一个矩阵的行数）
bool Matrix::canMultiply(const Matrix& other) const {
    return cols == other.rows;
}
