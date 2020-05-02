//直方图均衡化 真正的直接操作直方图的，不需要计算原始输入图像的数据
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <math.h>
#include <iostream>
using namespace std;

cv::Mat hist_equalize(cv::Mat img){
    //参考：https://blog.csdn.net/schwein_van/article/details/84336633
    //其实是灰度级值的变换，比如灰度级为2的像素有100个，这个灰度级均衡化后值变成了4，那么这100个像素点的像素值变换为了4
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    
    double* hist;
    hist = CalHist(img);
    int hist_equalize_val[256]; // 每个灰度级(这里就是每个像素值)对应的均衡化后的像素值
    double coff = 255.0/(height*width*channel);
    double accum_hist = 0;
    for(int i=0;i<256;i++){
        accum_hist += *(hist+i);
        hist_equalize_val[i] = (int)(coff * accum_hist);
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                int val = (int)img.at<cv::Vec3b>(i,j)[_c];
                out.at<cv::Vec3b>(i,j)[_c] = (uchar)hist_equalize_val[val];
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat img_eq = hist_equalize(img);
    cv::imshow("t23", img_eq);
    cv::waitKey(0);
    Hist(img_eq);
    cv::destroyAllWindows();
}
