//缩小与放大。。。
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = bilinear_interpolation(img, 0.25, 0.25);
    out = bilinear_interpolation(out, 4, 4);
    cv::imshow("t73", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
