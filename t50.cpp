//形态学运算——闭运算
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat closing_op(cv::Mat binary, int N=1){
    return erode(dilate(binary, N), N);
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat binary = Canny(img);
    cv::imshow("binary", binary);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat out = closing_op(binary, 1);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
