//形态学——顶帽
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat top_hat(cv::Mat img, int N=1){
    cv::Mat binary = Ostu_Threshold_two(img);
    cv::Mat open = opening_closing_op(binary, N, true);
    //return binary-open; //注意减的顺序，和black_hat的对比，因为开运算的输出是腐蚀为主，即255变为0，下面的做法则没有这个问题，因为32F可以为负数
    cv::Mat binary_HD;
    cv::Mat open_HD;
    binary.convertTo(binary_HD, CV_32F);
    open.convertTo(open_HD, CV_32F);
    return binary_HD-open_HD;
}

int main(){
    cv::Mat img = cv::imread("../imgs/bin.png", cv::IMREAD_COLOR);
    cv::Mat out = top_hat(img, 1);
    cout<<out.type()<<endl; //对应的结果https://blog.csdn.net/renhaofan/article/details/81207924
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
