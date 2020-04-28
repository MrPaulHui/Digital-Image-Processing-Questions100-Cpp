// HSV变换和反变换
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat RGB2HSV(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    float h, s, v;
    float r, g, b;
    float _max, _min;
    cv::Mat hsv = cv::Mat::zeros(height, width, CV_32FC3); //CV_32FC3表示每个像素值为32bit，F表示float类型，对应U表示unsigned int，C3表示3个通道
    for(int i=0;i<255;i++){
        for(int j=0;j<255;j++){
            r = img.at<cv::Vec3b>(i,j)[0]/255;
            g = img.at<cv::Vec3b>(i,j)[1]/255;
            b = img.at<cv::Vec3b>(i,j)[2]/255;
            _max = fmax(r, fmax(g, b));
            _min = fmin(r, fmin(g, b));
            // get h
            if(_max == _min){
                h = 0;
            } else if(_min == b){
                h = 60*(g-r)/(_max-_min)+60;
            } else if(_min == r){
                h = 60*(b-g)/(_max-_min)+180;
            } else if(_min == g){
                h = 60*(r-b)/(_max-_min)+300;
            }
            // get s
            s = _max - _min;
            // get v
            v = _max;
            hsv.at<cv::Vec3f>(i,j)[0] = h;
            hsv.at<cv::Vecef>(i,j)[1] = s;
            hsv.at<cv::Vec3f>(i,j)[2] = v;
        }
    }
    return hsv;
}
