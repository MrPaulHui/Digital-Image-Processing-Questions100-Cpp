//DFT 低通滤波
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat low_pass_filter(cv::Mat img, double pass_r=0.5){
    int height = img.rows;
    int width = img.cols;
    cv::Mat* fourier = DFT_advanced(img);
    double filter_d = (height/2)*pass_r;
    for(int i=0;i<height/2;i++){
        for(int j=0;j<width/2;j++){
            if(sqrt(i*i+j*j)>=filter_d){
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
    cv::Mat out = low_pass_filter(img, 0.4);
    cv::imshow("t33", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
