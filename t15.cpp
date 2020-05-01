//sobel边缘检测
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat gray = ToGray(img);
    //纵向
    double sobel_filter_row[3][3] = {
        {-1.0, -2.0, -1.0},
        {0, 0, 0},
        {1.0, 2.0, 1.0},
    };
    cv::Mat kernel_row(3,3,CV_64FC1,sobel_filter_row); //用具体值初始化Mat的方法，64F对应double，32F对应float
    //横向取横向左方向的邻接差值
    double sobel_filter_col[3][3] = {
        {-1.0, 0, 1.0},
        {-2.0, 0, 2.0},
        {-1.0, 0, 1.0},
    };
    cv::Mat kernel_col(3,3,CV_64FC1,sobel_filter_col); //用具体值初始化Mat的方法，64F对应double，32F对应float
    cv::Mat out_row = Conv_Gray(gray, kernel_row);
    cv::Mat out_col = Conv_Gray(gray, kernel_col); //C++似乎不能像python那样只对其中几个形参赋值？
    cv::Mat out = Clip(out_row + out_col); //合并横向和纵向的结果，下面注释掉的是分别输出
    cv::imshow("t15_", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    /*
    cv::Mat disp;
    cv::Mat img_list[3];
    img_list[0] = out_row;
    img_list[1] = cv::Mat::zeros(out_row.rows, out_row.cols, CV_8UC1);
    img_list[2] = out_col;
    cv::hconcat(img_list, 3, disp);
    cv::imshow("t15", disp);
    cv::waitKey(0);
    cv::destroyAllWindows();
    */
}
