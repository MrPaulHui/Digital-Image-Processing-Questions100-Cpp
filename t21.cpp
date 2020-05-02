//直方图归一化 本质就是对像素值范围进行线性映射，使像素值范围扩大，不集中于某一块。
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <math.h>
#include <iostream>
using namespace std;

cv::Mat hist_normalize(cv::Mat img, int a, int b){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    double c = 255;
    double d = 0;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                double val = (double)img.at<cv::Vec3b>(i,j)[_c];
                c = fmin(c, val);
                d = fmax(d, val);
            }
        }
    }
    double coff = (b-a)/(d-c);
    double v;
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                double val = (double)img.at<cv::Vec3b>(i,j)[_c];
                if(val<a){
                    v = a;
                }
                else if(val<b){
                    v = coff*(val-c)+a;
                }
                else{
                    v = b;
                }
                out.at<cv::Vec3b>(i,j)[_c] = (uchar)v;
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_dark.jpg", cv::IMREAD_COLOR);
    cv::Mat img_normed = hist_normalize(img,0,255);
    cv::imshow("t21", img_normed);
    cv::waitKey(0);
    Hist(img_normed);
    cv::destroyAllWindows();
}
