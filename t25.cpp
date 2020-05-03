//最近邻插值
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat nearest_neighbor_interpolation(cv::Mat img, double scale){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    int out_height = (int)(height*scale);
    int out_width = (int)(width*scale);
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC3);
    int ori_i, ori_j;
    for(int i=0;i<out_height;i++){
        ori_i = round(i/scale); //注意优化，这里没必要写到最小的循环里
        for(int j=0;j<out_width;j++){
            ori_j = round(j/scale);
            for(int c=0;c<channel;c++){
                out.at<cv::Vec3b>(i,j)[c] = img.at<cv::Vec3b>(ori_i,ori_j)[c];
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = nearest_neighbor_interpolation(img, 1.5);
    cv::imshow("t25", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
