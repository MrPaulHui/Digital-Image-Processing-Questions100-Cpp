//YCbCr色彩空间
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat BGR2YCbCr(cv::Mat bgr){
    int height = bgr.rows;
    int width = bgr.cols;
    cv::Mat ycbcr = cv::Mat::zeros(height, width, CV_64FC3);
    double b,g,r;
    double y,cb,cr;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            b = (double)bgr.at<cv::Vec3b>(i,j)[0];
            g = (double)bgr.at<cv::Vec3b>(i,j)[1];
            r = (double)bgr.at<cv::Vec3b>(i,j)[2];
            y = 0.299*r+0.5870*g+0.114*b;
            cb = -0.1687*r-0.3313*g+0.5*b+128;
            cr = 0.5*r-0.4187*g-0.0813*b+128;
            ycbcr.at<cv::Vec3d>(i,j)[0] = y;
            ycbcr.at<cv::Vec3d>(i,j)[1] = cb;
            ycbcr.at<cv::Vec3d>(i,j)[2] = cr;
        }
    }
    return ycbcr;
}

cv::Mat YCbCr2BGR(cv::Mat ycbcr){
    int height = ycbcr.rows;
    int width = ycbcr.cols;
    cv::Mat bgr = cv::Mat::zeros(height, width, CV_8UC3);
    double b,g,r;
    double y,cb,cr;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            y = ycbcr.at<cv::Vec3d>(i,j)[0];
            cb = ycbcr.at<cv::Vec3d>(i,j)[1];
            cr = ycbcr.at<cv::Vec3d>(i,j)[2];
            r = y+(cr-128)*1.402;
            g = y-(cb-128)*0.3441-(cr-128)*0.7139;
            b = y+(cb-128)*1.7718;
            bgr.at<cv::Vec3b>(i,j)[0] = b;
            bgr.at<cv::Vec3b>(i,j)[1] = g;
            bgr.at<cv::Vec3b>(i,j)[2] = r;
        }
    }
    return bgr;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat ycbcr = BGR2YCbCr(img);
    for(int i=0;i<ycbcr.rows;i++){
        for(int j=0;j<ycbcr.cols;j++){
            ycbcr.at<cv::Vec3d>(i,j)[0] = 0.7*ycbcr.at<cv::Vec3d>(i,j)[0];
        }
    }
    cv::Mat new_img = YCbCr2BGR(ycbcr);
    cv::imshow("img", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imshow("new_img", new_img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
