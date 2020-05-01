//emboss边缘检测
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat gray = ToGray(img);
    double emboss_filter[3][3] = {
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2},
    };
    cv::Mat kernel(3,3,CV_64FC1,emboss_filter); //用具体值初始化Mat的方法，64F对应double，32F对应float
    cv::Mat out = Conv_Gray(gray, kernel);
    cv::imshow("t18", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
