//透明混合
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

cv::Mat alpha_blending(cv::Mat img1, cv::Mat img2, double alpha=0.5){
    int height1 = img1.rows;
    int width1 = img1.cols;
    int height2 = img2.rows;
    int width2 = img2.cols;
    int channel = img1.channels();
    if(height1 != height2 || width1 != width2){
        img2 = bilinear_interpolation(img2, double(height1)/double(height2), double(width1)/double(width2));
    }
    cv::Mat out = cv::Mat::zeros(height1, width1, CV_8UC3);
    for(int i=0;i<height1;i++){
        for(int j=0;j<width1;j++){
            for(int c=0;c<channel;c++){
                out.at<cv::Vec3b>(i,j)[c] = img1.at<cv::Vec3b>(i,j)[c]*alpha + img2.at<cv::Vec3b>(i,j)[c]*(1-alpha);
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img1 = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread("../imgs/thorino.jpg", cv::IMREAD_COLOR);
    cv::Mat out = alpha_blending(img1, img2, 0.6);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
