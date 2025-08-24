#include "RedEdgeDetector.h"
#include <iostream>

int main() {
    RedEdgeDetector detector;
    
    // 示例调用 - 调试模式，不保存视频
    if (!detector.initialize("R2.avi", "R2_detected.mp4",false)) {
        std::cerr << "初始化失败" << std::endl;
        return -1;
    }
    
    detector.processVideo();
    detector.release();
    
    return 0;
}
