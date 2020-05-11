//形态学运算——形态学梯度
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat morphology_gradient(cv::Mat img, int N=1){
    cv::Mat binary = Ostu_Threshold_two(img);
    cv::Mat dilation = dilate(binary, N);
    cv::Mat erosion = erode(binary, N);
    return dilation-erosion;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = morphology_gradient(img, 1);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
