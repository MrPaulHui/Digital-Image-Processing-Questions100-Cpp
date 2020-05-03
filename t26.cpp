//双线性插值
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat bilinear_interpolation(cv::Mat img, double scale){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    int out_height = (int)(height*scale);
    int out_width = (int)(width*scale);
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC3);
    int x, y;
    double dx, dy;
    double val;
    for(int i=0;i<out_height;i++){
        y = floor(i/scale);
        dy = i/scale - y;
        for(int j=0;j<out_width;j++){
            x = floor(j/scale);
            dx = j/scale - x;
            for(int c=0;c<channel;c++){
                val = (1-dx)*(1-dy)*(double)img.at<cv::Vec3b>(y,x)[c] + dx*(1-dy)*(double)img.at<cv::Vec3b>(y,x+1)[c] + 
                      (1-dx)*dy*(double)img.at<cv::Vec3b>(y+1,x)[c] + dx*dy*(double)img.at<cv::Vec3b>(y+1,x+1)[c];
                out.at<cv::Vec3b>(i,j)[c] = (uchar)val;
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = bilinear_interpolation(img, 1.5);
    cv::imshow("t26", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
