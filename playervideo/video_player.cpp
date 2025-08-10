#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // 检查是否提供了视频文件路径
    if (argc < 2) {
        std::cerr << "请提供视频文件路径作为参数" << std::endl;
        return -1;
    }

    // 创建窗口
    cv::namedWindow("Example2", cv::WINDOW_AUTOSIZE);
    
    // 打开视频文件
    cv::VideoCapture capture(argv[1]);
    if (!capture.isOpened()) {  // 检查视频是否成功打开
        std::cerr << "无法打开视频文件: " << argv[1] << std::endl;
        return -1;
    }

    cv::Mat frame;  

    while (true) {
        // 读取一帧
        capture >> frame;
        
        // 检查是否到达视频末尾
        if (frame.empty()) break;
        
        // 显示帧
        cv::imshow("Example2", frame);
        
        // 如果按下ESC键退出
        char c = cv::waitKey(1);
        if (c == 27) break;
    }

    
    cv::destroyWindow("Example2");
    return 0;
}
