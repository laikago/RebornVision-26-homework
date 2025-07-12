#include "matrix.h"
#include <iostream>

int main() {
    try {
        // 1. 矩阵创建演示
        std::cout << "=== 测试矩阵创建 ===\n";
        
        // 测试初始值构造函数
        Matrix m1(2, 3, 1.0);
        std::cout << "m1 (2x3, 全1):\n" << m1;
        
        Matrix m2(2, 3, 2.0);
        std::cout << "m2 (2x3, 全2):\n" << m2;
        
        // 测试向量构造函数
        std::vector<std::vector<double>> vec = {{1,1}, {1,1}};
        Matrix m3(vec);
        std::cout << "m3 (从向量创建 2x2, 全1):\n" << m3;
        
        Matrix m4(2, 2, 2.0);
        std::cout << "m4 (2x2, 全2):\n" << m4;


        // 2. 矩阵加法演示
        std::cout << "\n=== 测试矩阵加法 ===\n";
        Matrix m_add = m3 + m4;  // 2x2 + 2x2
        std::cout << "m3:\n" << m3;
        std::cout << "m4:\n" << m4;
        std::cout << "m3 + m4:\n" << m_add;


        // 3. 矩阵乘法演示
        std::cout << "\n=== 测试矩阵乘法 ===\n";
        Matrix m5(2, 3, 1.0);  // 2x3
        Matrix m6(3, 2, 2.0);  // 3x2（与m5可乘：3列 vs 3行）
        Matrix m_mul = m5 * m6;  // 结果为 2x2
        std::cout << "m5 (2x3, 全1):\n" << m5;
        std::cout << "m6 (3x2, 全2):\n" << m6;
        std::cout << "m5 * m6:\n" << m_mul;


        // 4. 错误处理演示（维度不匹配的加法）
        std::cout << "\n=== 测试错误处理 ===\n";
        std::cout << "尝试 m1 + m3 (维度不匹配):\n";
        try {
            Matrix err_add = m1 + m3;  // m1是2x3，m3是2x2，无法相加
        } catch (const MatrixException& e) {
            std::cout << "错误: " << e.what() << "\n";
        }


        // 5. 安全访问演示
        std::cout << "\n=== 测试安全访问 ===\n";
        std::cout << "m3的(0,0)元素: " << m3.at(0, 0) << "\n";
        std::cout << "尝试访问m3的(2,0)元素（越界）:\n";
        try {
            m3.at(2, 0);
        } catch (const MatrixException& e) {
            std::cout << "错误: " << e.what() << "\n";
        }

    } catch (const MatrixException& e) {
        std::cout << "程序错误: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
