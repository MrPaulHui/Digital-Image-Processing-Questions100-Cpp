//Laplacian二阶边缘检测
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat gray = ToGray(img);
    double laplacian_filter[3][3] = {
        {0, 1, 0},
        {1, -4, 1},
        {0, 1, 0},
    };
    cv::Mat kernel(3,3,CV_64FC1,laplacian_filter); //用具体值初始化Mat的方法，64F对应double，32F对应float
    cv::Mat out = Conv_Gray(gray, kernel);
    cv::imshow("t17", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
