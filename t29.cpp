//仿射变换——放大缩小
//从放大缩小这个仿射变换更看出用反变换实现的重要性，因为直接正变换的化，输出的像素点有的对应不回去，即没有被原图像素点对应到
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out1 = Affine(img, 1.3,0,0,0.8,0,0);
    cv::imshow("t29_1", out1);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat out2 = Affine(img, 1.3,0,0,0.8,30,-30);
    cv::imshow("t29_2", out2);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
