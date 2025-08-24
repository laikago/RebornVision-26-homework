#include "RedEdgeDetector.h"
#include <iostream>
#include <iomanip>

RedEdgeDetector::RedEdgeDetector() 
    : frameWidth(0), frameHeight(0), fps(0), totalFrames(0), frameCount(0),
      debugMode(false), initialized(false),
      lowerRed1(0, 120, 70), upperRed1(10, 255, 255),
      lowerRed2(170, 120, 70), upperRed2(180, 255, 255),
      threshold_x(5), threshold_y(3) {
    kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
}

RedEdgeDetector::~RedEdgeDetector() {
    release();
}

bool RedEdgeDetector::initialize(const std::string& inputPath, const std::string& outputPath, bool debug) {
    debugMode = debug;
    
    // 打开视频文件
    cap.open(inputPath);
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频文件: " << inputPath << std::endl;
        return false;
    }
    
    // 获取视频信息
    frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps = cap.get(cv::CAP_PROP_FPS);
    totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    
    // 创建VideoWriter对象
    if (!debugMode && !outputPath.empty()) {
        int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
        out.open(outputPath, fourcc, fps, cv::Size(frameWidth, frameHeight));
        if (!out.isOpened()) {
            std::cerr << "无法创建输出视频文件: " << outputPath << std::endl;
            return false;
        }
    }
    
    // 创建显示窗口
    cv::namedWindow("原始视频", cv::WINDOW_NORMAL);
    cv::namedWindow("红色区域二值图", cv::WINDOW_NORMAL);
    cv::namedWindow("中点", cv::WINDOW_NORMAL);
    cv::namedWindow("提取的直线", cv::WINDOW_NORMAL);
    
    // 调整窗口位置
    cv::moveWindow("原始视频", 100, 100);
    cv::moveWindow("红色区域二值图", 800, 100);
    cv::moveWindow("中点", 100, 500);
    cv::moveWindow("提取的直线", 800, 500);
    
    std::cout << "开始处理视频..." << std::endl;
    std::cout << "模式: " << (debugMode ? "调试模式（不保存视频）" : "正式模式（保存视频）") << std::endl;
    
    frameCount = 0;
    initialized = true;
    return true;
}

void RedEdgeDetector::extractRedRegions(const cv::Mat& frame, cv::Mat& redMask) {
    cv::Mat hsv, mask1, mask2;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, lowerRed1, upperRed1, mask1);
    cv::inRange(hsv, lowerRed2, upperRed2, mask2);
    redMask = mask1 | mask2;  // 合并红色区域
    
    // 形态学去噪
    cv::morphologyEx(redMask, redMask, cv::MORPH_CLOSE, kernel2);
    cv::morphologyEx(redMask, redMask, cv::MORPH_OPEN, kernel1);
}

void RedEdgeDetector::detectEdges(const cv::Mat& redMask, cv::Mat& diff) {
    diff = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC1);
    //
    linesFinal.clear();
    for (int y = 0; y < frameHeight; ++y) {
        bool flag = false;
        int leftedge = 0, rightedge = 0, mid = 0;
        
        const uchar* rowPtr = redMask.ptr<uchar>(y);
        uchar* diffPtr = diff.ptr<uchar>(y);
        
        // 处理第一列
        if (rowPtr[0] == 255) {
            leftedge = 0;
            flag = true;
        }
        
        // 计算水平差分
        for (int x = 1; x < frameWidth; ++x) {
            if (rowPtr[x] > rowPtr[x - 1]) {
                flag = true;
                leftedge = x;
            } else if (rowPtr[x] < rowPtr[x - 1]) {
                flag = false;
                rightedge = x;
                mid = (leftedge + rightedge) / 2;
                diffPtr[mid] = 255;
                
                // 尝试将中点添加到现有线段
                bool merged = false;
                for (size_t i = 0; i < lines.size(); ++i) {
                    if (std::abs(mid - lines[i].end_x) < threshold_x) {
                        lines[i].end_x = mid;
                        lines[i].end_y = y;
                        merged = true;
                        break;
                    }
                }
                
                // 如果没有合并到现有线段，创建新线段
                if (!merged) {
                    lines.push_back(MyLine(mid, y, mid, y));
                }
            }
        }
        //
        for (size_t i = 0; i < lines.size(); i++) {
        if (y-lines[i].end_y>threshold_y) {
            if (lines[i].end_y - lines[i].start_y > 10) {
                linesFinal.push_back(lines[i]);
            }
            lines.erase(lines.begin() + i);
            i--;  // 调整索引
        }
    }
        
    }
}

/*void RedEdgeDetector::findLines(const cv::Mat& diff) {
    linesFinal.clear();
    
    // 筛选符合条件的线段
    for (size_t i = 0; i < lines.size(); i++) {
        if (y-lines[i].end_y>threshold_y) {
            if (lines[i].end_y - lines[i].start_y > 10) {
                linesFinal.push_back(lines[i]);
            }
            lines.erase(lines.begin() + i);
            i--;  // 调整索引
        }
    }
}
*/
void RedEdgeDetector::matchLightBars(cv::Mat& frame) {
    if (linesFinal.size() >= 2) {
        int index1 = 0, index2 = 1;
        int min_d = 1000;
        
        // 找到最接近的一对线段
        for (size_t i = 0; i < linesFinal.size(); ++i) {
            int x1 = linesFinal[i].start_x;
            int y1 = linesFinal[i].start_y;
            
            for (size_t j = i + 1; j < linesFinal.size(); ++j) {
                int x2 = linesFinal[j].start_x;
                int y2 = linesFinal[j].start_y;
                
                int dx = std::abs(x2 - x1);
                int dy = std::abs(y2 - y1);
                int distance = dx + dy * dy;
                
                if (distance < min_d) {
                    min_d = distance;
                    index1 = i;
                    index2 = j;
                }
            }
        }
        
        // 绘制匹配的灯条
        cv::line(frame, 
                 cv::Point(linesFinal[index1].start_x, linesFinal[index1].start_y), 
                 cv::Point(linesFinal[index2].start_x, linesFinal[index2].start_y), 
                 cv::Scalar(255, 0, 0), 2);
        
        cv::line(frame, 
                 cv::Point(linesFinal[index2].end_x, linesFinal[index2].end_y), 
                 cv::Point(linesFinal[index1].end_x, linesFinal[index1].end_y), 
                 cv::Scalar(255, 0, 0), 2);
        
        // 添加标签
        cv::putText(frame, "1", 
                    cv::Point(linesFinal[index1].start_x, linesFinal[index1].start_y), 
                    cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 2);
        
        cv::putText(frame, "2", 
                    cv::Point(linesFinal[index2].start_x, linesFinal[index2].start_y),
                    cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 2);
        
        cv::putText(frame, "4", 
                    cv::Point(linesFinal[index1].end_x, linesFinal[index1].end_y),
                    cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 2);
        
        cv::putText(frame, "3", 
                    cv::Point(linesFinal[index2].end_x, linesFinal[index2].end_y), 
                    cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 2);
    }
}

void RedEdgeDetector::drawResults(cv::Mat& frame) {
    // 绘制所有检测到的线段
    for (const auto& line : linesFinal) {
        cv::line(frame, 
                 cv::Point(line.start_x, line.start_y), 
                 cv::Point(line.end_x, line.end_y), 
                 cv::Scalar(0, 255, 0), 2);
    }
}

void RedEdgeDetector::processVideo() {
    if (!initialized) {
        std::cerr << "检测器未初始化" << std::endl;
        return;
    }
    
    cv::Mat frame, redMask, diff, resultFrame;
    
    while (cap.read(frame)) {
        // 1. 提取红色区域
        extractRedRegions(frame, redMask);
        
        // 2. 边缘检测
        detectEdges(redMask, diff);
        
        // 3. 查找线段
        //findLines(diff);
        
        // 4. 创建结果帧
        resultFrame = frame.clone();
        
        // 5. 绘制结果
        drawResults(resultFrame);
        
        // 6. 匹配灯条
        matchLightBars(resultFrame);
        
        // 7. 保存与显示
        if (!debugMode && out.isOpened()) {
            out.write(resultFrame);
        }
        
        // 显示进度
        frameCount++;
        if (frameCount % 10 == 0) {
            double progress = (static_cast<double>(frameCount) / totalFrames) * 100;
            std::cout << "\r处理进度: " << std::fixed << std::setprecision(2) << progress << "%" << std::flush;
        }
        
        // 显示窗口
        cv::imshow("原始视频", frame);
        cv::imshow("红色区域二值图", redMask);
        cv::imshow("中点", diff);
        cv::imshow("提取的直线", resultFrame);
        
        // 按下q键退出
        if (cv::waitKey(1) == 'q') {
            std::cout << "\n用户中断处理" << std::endl;
            break;
        }
    }
    
    std::cout << "\n视频处理完成" << std::endl;
}

void RedEdgeDetector::release() {
    if (cap.isOpened()) {
        cap.release();
    }
    
    if (out.isOpened()) {
        out.release();
    }
    
    cv::destroyAllWindows();
    initialized = false;
}
