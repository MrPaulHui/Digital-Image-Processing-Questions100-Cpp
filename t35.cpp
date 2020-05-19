//DFT 高通滤波
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat band_pass_filter(cv::Mat img, double low_pass_r=0.1, double high_pass_r=0.5){
    int height = img.rows;
    int width = img.cols;
    cv::Mat* fourier = DFT_advanced(img);
    double filter_d_low = (height/2)*low_pass_r;
    double filter_d_high = (height/2)*high_pass_r;
    for(int i=0;i<height/2;i++){
        for(int j=0;j<width/2;j++){
            if(sqrt(i*i+j*j)<=filter_d_low || sqrt(i*i+j*j)>=filter_d_high){
                (*fourier).at<double>(i,j) = 0;
                (*(fourier+1)).at<double>(i,j) = 0;
                (*fourier).at<double>(i,width-1-j) = 0;
                (*(fourier+1)).at<double>(i,width-1-j) = 0;
                (*fourier).at<double>(height-1-i,j) = 0;
                (*(fourier+1)).at<double>(height-1-i,j) = 0;
                (*fourier).at<double>(height-1-i,width-1-j) = 0;
                (*(fourier+1)).at<double>(height-1-i,width-1-j) = 0;
            }
        }
    }
    return IDFT_advanced(fourier);
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = band_pass_filter(img);
    cv::imshow("t35", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
