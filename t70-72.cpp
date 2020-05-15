//色彩追踪 color tracking
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat color_tracking(cv::Mat img, int low_thresh=180, int high_thresh=260){
    //目前只能追踪蓝色，若知道其他颜色范围，也可以同理实现
    //返回二进制mask图像，追踪到指定颜色的部分为255，其他为0
    int height = img.rows;
    int width = img.cols;
    cv::Mat hsv = RGB2HSV(img);
    cv::Mat mask = cv::Mat::zeros(height, width, CV_8UC1);
    double hue;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            hue = (double)hsv.at<cv::Vec3f>(i,j)[0];
            if(hue>low_thresh && hue<high_thresh){
                mask.at<uchar>(i,j) = 255;
            }
        }
    }
    return mask;
}

cv::Mat color_tracking_mask(cv::Mat img, int low_thresh=180, int high_thresh=260){
    int height = img.rows;
    int width = img.cols;
    cv::Mat mask = color_tracking(img,low_thresh,high_thresh);
    cv::Mat new_mask = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(mask.at<uchar>(i,j)==0){
                new_mask.at<cv::Vec3b>(i,j) = cv::Vec3b(1,1,1);
            }
        }
    }
    return img.mul(new_mask); //对应相乘库函数使用：https://blog.csdn.net/dcrmg/article/details/52404580
}

cv::Mat color_tracking_mask_Morphology(cv::Mat img, int low_thresh=180, int high_thresh=260, int N=5){
    //对mask先进行形态学处理，即开运算和闭运算，去除孤立点等
    int height = img.rows;
    int width = img.cols;
    cv::Mat mask = color_tracking(img,low_thresh,high_thresh);
    mask = opening_closing_op(mask, N, false);
    mask = opening_closing_op(mask, N, true);
    cv::imshow("mask_after_morphology", mask);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat new_mask = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(mask.at<uchar>(i,j)==0){
                new_mask.at<cv::Vec3b>(i,j) = cv::Vec3b(1,1,1);
            }
        }
    }
    return img.mul(new_mask);
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat mask = color_tracking(img);
    cv::imshow("t70", mask);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat out1 = color_tracking_mask(img);
    cv::imshow("t71", out1);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat out2 = color_tracking_mask_Morphology(img);
    cv::imshow("t72", out2);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
