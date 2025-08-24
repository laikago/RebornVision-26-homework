#ifndef RED_EDGE_DETECTOR_H
#define RED_EDGE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct MyLine {
    int start_x, start_y, end_x, end_y;
    MyLine() : start_x(0), start_y(0), end_x(0), end_y(0) {}
    MyLine(int a, int b, int c, int d) : start_x(a), start_y(b), end_x(c), end_y(d) {}
};

class RedEdgeDetector {
public:
    RedEdgeDetector();
    ~RedEdgeDetector();
    
    bool initialize(const std::string& inputPath, const std::string& outputPath = "", bool debugMode = false);
    void processVideo();
    void release();
    
private:
    void extractRedRegions(const cv::Mat& frame, cv::Mat& redMask);
    void detectEdges(const cv::Mat& redMask, cv::Mat& diff);
    void findLines(const cv::Mat& diff);
    void matchLightBars(cv::Mat& frame);
    void drawResults(cv::Mat& frame);
    
    cv::VideoCapture cap;
    cv::VideoWriter out;
    
    std::vector<MyLine> lines;
    std::vector<MyLine> linesFinal;
    
    int frameWidth;
    int frameHeight;
    double fps;
    int totalFrames;
    int frameCount;
    
    bool debugMode;
    bool initialized;
    
    // HSV红色范围
    cv::Scalar lowerRed1;
    cv::Scalar upperRed1;
    cv::Scalar lowerRed2;
    cv::Scalar upperRed2;
    
    // 形态学核
    cv::Mat kernel1;
    cv::Mat kernel2;
    
    // 阈值参数
    int threshold_x;
    int threshold_y;
};

#endif // RED_EDGE_DETECTOR_H
