//均值滤波
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

cv::Mat Mean_Filter(cv::Mat img, int k_size=3, bool need_pad=true){
    //中值滤波不是卷积操作，而是取周围3*3共9个值的中值，把这个中值直接赋给这个中心像素点
    //均值滤波和中值滤波类似，也不是卷积操作，取3*3的平均值，把这个平均值赋给这个中心像素点
    if(need_pad){
        img = ZeroPadding(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height-k_size+1,width-k_size+1,CV_8UC3);
    double mean;
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            for(int c=0;c<channel;c++){
                mean = 0;
                for(int ii=0;ii<k_size;ii++){
                    for(int jj=0;jj<k_size;jj++){
                        mean += (double)img.at<cv::Vec3b>(i+ii,j+jj)[c];
                    }
                }
                mean /= (k_size*k_size);
                out.at<cv::Vec3b>(i,j)[c] = (uchar)mean;
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_noise.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Mean_Filter(img);
    cv::imshow("t11", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
