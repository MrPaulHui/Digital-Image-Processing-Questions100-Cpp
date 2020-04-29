//motion filter 对图像进行运动模糊处理
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    //motion filter取对角线的平均值作为输出像素值
    double motion_filter[3][3] = {
        {1.0/3.0, 0, 0},
        {0, 1.0/3.0, 0},
        {0, 0, 1.0/3.0}, //注意这里必须加.0，因为c++里整数相除默认为整除
    };
    cv::Mat kernel(3,3,CV_64FC1,motion_filter); //用具体值初始化Mat的方法，64F对应double，32F对应float
    cv::Mat out = Conv(img,kernel);
    cv::imshow("t12", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
