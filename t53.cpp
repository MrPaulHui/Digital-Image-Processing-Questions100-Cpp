//形态学——黑帽
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat black_hat(cv::Mat img, int N=1){
    cv::Mat binary = Ostu_Threshold_two(img);
    cv::Mat close = opening_closing_op(binary, N, false);
    //return close-binary; //注意减的顺序，和top_hat的对比，闭运算的输出是膨胀为主，即0变成255，下面的做法则没有这个问题，因为32F可以为负数
    cv::Mat binary_HD;
    cv::Mat close_HD;
    binary.convertTo(binary_HD, CV_32F);
    close.convertTo(close_HD, CV_32F);
    return binary_HD-close_HD;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = black_hat(img, 1);
    cout<<out.type()<<endl; //对应的结果https://blog.csdn.net/renhaofan/article/details/81207924
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
