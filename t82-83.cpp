//Harris角点检测
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
#include <math.h>
using namespace std;

cv::Mat harris_corner(cv::Mat img, double k=0.04, double thresh=0.1, bool disp_middle=true){
    cv::Mat gray = ToGray(img);
    double sobel_filter_row[3][3] = {
        {-1.0, -2.0, -1.0},
        {0, 0, 0},
        {1.0, 2.0, 1.0},
    };
    cv::Mat sobel_kernel_row(3,3,CV_64FC1,sobel_filter_row);
    double sobel_filter_col[3][3] = {
        {-1.0, 0, 1.0},
        {-2.0, 0, 2.0},
        {-1.0, 0, 1.0},
    };
    cv::Mat sobel_kernel_col(3,3,CV_64FC1,sobel_filter_col);
    cv::Mat Ix = Conv_Gray_HD(gray, sobel_kernel_row);
    cv::Mat Iy = Conv_Gray_HD(gray, sobel_kernel_col);
    cv::Mat Ix2 = Ix.mul(Ix);
    cv::Mat Iy2 = Iy.mul(Iy);
    cv::Mat Ixy = Ix.mul(Iy);
    Ix2 = Guassian_Filter(Ix2, 3, 3, true);
    Iy2 = Guassian_Filter(Iy2, 3, 3, true);
    Ixy = Guassian_Filter(Ixy, 3, 3, true);
    if(disp_middle){
        //这段又臭又长的代码，可以删掉，但就留着吧
        cv::Mat Ix2_disp, Iy2_disp, Ixy_disp;
        double minval, maxval;
        cv::minMaxIdx(Ix2, &minval, &maxval);
        cv::Mat Ix2_ = Ix2/maxval*255;
        cv::minMaxIdx(Iy2, &minval, &maxval);
        cv::Mat Iy2_ = Iy2/maxval*255;
        cv::minMaxIdx(Ixy, &minval, &maxval);
        cv::Mat Ixy_ = Ixy/maxval*255;
        Ix2_.convertTo(Ix2_disp, CV_8U);
        Iy2_.convertTo(Iy2_disp, CV_8U);
        Ixy_.convertTo(Ixy_disp, CV_8U);
        cv::imshow("Ix2_disp", Ix2_disp);
        cv::waitKey(0);
        cv::destroyAllWindows();
        cv::imshow("Iy2_disp", Iy2_disp);
        cv::waitKey(0);
        cv::destroyAllWindows();
        cv::imshow("Ixy_disp", Ixy_disp);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    cv::Mat R = Ix2.mul(Iy2) - Ixy.mul(Ixy) - k * (Ix2+Iy2).mul(Ix2+Iy2);
    double maxR, minR;
    cv::minMaxIdx(R, &minR, &maxR);
    cv::Mat out = img.clone();
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(R.at<double>(i,j)>maxR*thresh){
                out.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,255);
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/thorino.jpg", cv::IMREAD_COLOR);
    cv::Mat corner_map = harris_corner(img);
    cv::imshow("corner_map", corner_map);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
