//形态学运算——开运算
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat opening_op(cv::Mat binary, int N=1){
    return dilate(erode(binary, N), N);
}

int main(){
    cv::Mat img = cv::imread("../imgs/digit.png", cv::IMREAD_COLOR);
    cv::Mat binary = Ostu_Threshold_two(img);
    cv::imshow("binary", binary);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat out = opening_op(binary, 5);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
