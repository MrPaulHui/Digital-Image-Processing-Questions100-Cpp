//题目描述：
//二值化
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
//#include "common.h"
using namespace std;

cv::Mat ToGray(cv::Mat img){
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC1);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            unsigned char r = img.at<cv::Vec3b>(i,j)[0];
            unsigned char g = img.at<cv::Vec3b>(i,j)[1];
            unsigned char b = img.at<cv::Vec3b>(i,j)[2];
            out.at<unsigned char>(i,j) = 0.2126 * r + 0.7152 * g + 0.0722 * b; //注意这里at后面尖括号的类型，很重要，这个类型是由访问到的元素的类型所决定的。
            // uchar = unsigned char
        }
    }
    return out;
}

cv::Mat Threshold_two(cv::Mat img){
    cv::Mat gray = ToGray(img);
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC1);
    for(int i=0; i<h; i++){
        for(int j=0; j<h; j++){
            if(gray.at<uchar>(i,j) >= 128){
                out.at<uchar>(i,j) = 255;
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Threshold_two(img);
    cv::imshow("t3", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
