//连接数
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat connected_number_4(cv::Mat img_){
    int height = img_.rows;
    int width = img_.cols;
    cv::Mat img;
    if(img_.channels()==3){
        img = Ostu_Threshold_two(img_);
    }
    else{
        img = img_.clone();
    }
    cv::Mat S_img = cv::Mat::zeros(height, width, CV_8UC1);
    double x1,x2,x3,x4,x5,x6,x7,x8;
    int S;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(img.at<uchar>(i,j)==0) continue;
            x1 = (double)img.at<uchar>(i,fmin(j+1,width-1))/255; //要时刻注意边界情况
            x2 = (double)img.at<uchar>(fmax(i-1,0),fmin(j+1,width-1))/255;
            x3 = (double)img.at<uchar>(fmax(i-1,0),j)/255;
            x4 = (double)img.at<uchar>(fmax(i-1,0),fmax(j-1,0))/255;
            x5 = (double)img.at<uchar>(i,fmax(j-1,0))/255;
            x6 = (double)img.at<uchar>(fmin(i+1,height-1),fmax(j-1,0))/255;
            x7 = (double)img.at<uchar>(fmin(i+1,height-1),j)/255;
            x8 = (double)img.at<uchar>(fmin(i+1,height-1),fmin(j+1,width-1))/255;
            S = (x1-x1*x2*x3)+(x3-x3*x4*x5)+(x5-x5*x6*x7)+(x7-x7*x8*x1);
            S_img.at<uchar>(i,j) = S+1;
        }
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    cv::Vec3b colors[6] = {cv::Vec3b(0,0,0),cv::Vec3b(0,0,255),cv::Vec3b(0,255,0),
                           cv::Vec3b(255,0,0),cv::Vec3b(255,255,0),cv::Vec3b(255,0,255)};
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            out.at<cv::Vec3b>(i,j) = colors[S_img.at<uchar>(i,j)];
        }
    }
    return out;
}

cv::Mat connected_number_8(cv::Mat img_){
    int height = img_.rows;
    int width = img_.cols;
    cv::Mat img;
    if(img_.channels()==3){
        img = Ostu_Threshold_two(img_);
    }
    else{
        img = img_.clone();
    }
    cv::Mat S_img = cv::Mat::zeros(height, width, CV_8UC1);
    int x1,x2,x3,x4,x5,x6,x7,x8;
    int S;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(img.at<uchar>(i,j)==0) continue;
            x1 = 1-(img.at<uchar>(i,fmin(j+1,width-1))/255); //要时刻注意边界情况
            x2 = 1-(img.at<uchar>(fmax(i-1,0),fmin(j+1,width-1))/255);
            x3 = 1-(img.at<uchar>(fmax(i-1,0),j)/255);
            x4 = 1-(img.at<uchar>(fmax(i-1,0),fmax(j-1,0))/255);
            x5 = 1-(img.at<uchar>(i,fmax(j-1,0))/255);
            x6 = 1-(img.at<uchar>(fmin(i+1,height-1),fmax(j-1,0))/255);
            x7 = 1-(img.at<uchar>(fmin(i+1,height-1),j)/255);
            x8 = 1-(img.at<uchar>(fmin(i+1,height-1),fmin(j+1,width-1))/255);
            S = (x1-x1*x2*x3)+(x3-x3*x4*x5)+(x5-x5*x6*x7)+(x7-x7*x8*x1);
            S_img.at<uchar>(i,j) = S+1;
        }
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    cv::Vec3b colors[6] = {cv::Vec3b(0,0,0),cv::Vec3b(0,0,255),cv::Vec3b(0,255,0),
                           cv::Vec3b(255,0,0),cv::Vec3b(255,255,0),cv::Vec3b(255,0,255)};
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            out.at<cv::Vec3b>(i,j) = colors[S_img.at<uchar>(i,j)];
        }
    }
    return out;
}

inline int connected_number_pixel(cv::Mat img, int i, int j, bool four=true){
    int height = img.rows;
    int width = img.cols;
    int x1,x2,x3,x4,x5,x6,x7,x8;
    int S;
    x1 = img.at<uchar>(i,fmin(j+1,width-1))/255; //要时刻注意边界情况
    x2 = img.at<uchar>(fmax(i-1,0),fmin(j+1,width-1))/255;
    x3 = img.at<uchar>(fmax(i-1,0),j)/255;
    x4 = img.at<uchar>(fmax(i-1,0),fmax(j-1,0))/255;
    x5 = img.at<uchar>(i,fmax(j-1,0))/255;
    x6 = img.at<uchar>(fmin(i+1,height-1),fmax(j-1,0))/255;
    x7 = img.at<uchar>(fmin(i+1,height-1),j)/255;
    x8 = img.at<uchar>(fmin(i+1,height-1),fmin(j+1,width-1))/255;
    if(four==false){
        x1 = 1-x1;
        x2 = 1-x2;
        x3 = 1-x3;
        x4 = 1-x4;
        x5 = 1-x5;
        x6 = 1-x6;
        x7 = 1-x7;
        x8 = 1-x8;
    }
    S = (x1-x1*x2*x3)+(x3-x3*x4*x5)+(x5-x5*x6*x7)+(x7-x7*x8*x1);
    return S;
}

int main(){
    cv::Mat img = cv::imread("../imgs/renketsu.png", cv::IMREAD_COLOR);
    cv::Mat out = connected_number_8(img);
    cv::imwrite("../img_result/renkestu_out.png", out);
    cv::imshow("t61-62", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
