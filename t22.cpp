//改变直方图的平均值和标准差，目的是让直方图变得平坦。
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <math.h>
#include <iostream>
using namespace std;

cv::Mat hist_transform(cv::Mat img, double m0, double s0){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    double sum = 0;
    double sqr_sum = 0;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                double val = (double)img.at<cv::Vec3b>(i,j)[_c];
                sum += val;
                sqr_sum += (val*val);
            }
        }
    }
    double m = sum / (height*width*channel);
    double s = sqrt(sqr_sum / (height*width*channel) - m*m);
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                double val = (double)img.at<cv::Vec3b>(i,j)[_c];
                double v = s0/s*(val-m)+m0; //变换公式
                out.at<cv::Vec3b>(i,j)[_c] = (uchar)v;
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_dark.jpg", cv::IMREAD_COLOR);
    cv::Mat img_trans = hist_transform(img,128,52);
    cv::imshow("t22", img_trans);
    cv::waitKey(0);
    Hist(img_trans);
    cv::destroyAllWindows();
}
