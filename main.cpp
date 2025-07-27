#include "matrix.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== 测试矩阵创建 ===\n";
        Matrix m1(2, 3, 1.0);
        std::cout << "m1 (2x3, 全1):\n" << m1;
        
        Matrix m2(2, 3, 2.0);
        std::cout << "m2 (2x3, 全2):\n" << m2;

        std::cout << "\n=== 测试矩阵加法 ===\n";
        Matrix sum = m1 + m2;
        std::cout << "m1 + m2:\n" << sum;

        std::cout << "\n=== 测试矩阵乘法 ===\n";
        Matrix m3(2, 2, 1.0);
        Matrix m4(2, 2, 2.0);
        std::cout << "m3:\n" << m3;
        std::cout << "m4:\n" << m4;
        std::cout << "m3 * m4:\n" << m3 * m4;

        std::cout << "\n=== 测试不同维度矩阵乘法 ===\n";
        Matrix m5(2, 3, 2.0);
        Matrix m6(3, 1, 4.0);
        std::cout << "m5:\n" << m5;
        std::cout << "m6:\n" << m6;
        std::cout << "m5 * m6:\n" << m5 * m6;

        std::cout << "\n=== 测试错误处理 ===\n";
        std::cout << "尝试 m1 + m5 (维度不匹配):\n";
        Matrix errorTest = m1 + m5;
    } catch (const MatrixException& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "未知异常发生!" << std::endl;
    }

    return 0;
}    