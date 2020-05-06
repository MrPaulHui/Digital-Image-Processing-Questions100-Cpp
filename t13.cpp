// max-min filter 用来提取边缘（效果应该不好吧？）
//下面这几题关于边缘检测的，都是只输出了梯度图（一阶或二阶），没有设定梯度阈值进行二值化
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

cv::Mat MaxMin_Filter(cv::Mat img, int k_size=3, bool need_pad=true){
    //中值滤波不是卷积操作，而是取周围3*3共9个值的中值，把这个中值直接赋给这个中心像素点
    //均值滤波和中值滤波类似，也不是卷积操作，取3*3的平均值，把这个平均值赋给这个中心像素点
    //max-min滤波操作方式也是，不过上两个作用是去噪，这个是提取边缘
    if(need_pad){
        img = ZeroPadding_Gray(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    //int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height-k_size+1,width-k_size+1,CV_8UC1);
    double max_, min_;
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            max_ = 0;
            min_ = 999;
            for(int ii=0;ii<k_size;ii++){
                for(int jj=0;jj<k_size;jj++){
                    if((double)img.at<uchar>(i+ii,j+jj) > max_){
                        max_ = (double)img.at<uchar>(i+ii,j+jj);
                    }
                    else if((double)img.at<uchar>(i+ii,j+jj) < min_){
                        min_ = (double)img.at<uchar>(i+ii,j+jj);
                    }
                }
            }
            out.at<uchar>(i,j) = (uchar)(max_-min_);
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat img2 = ZeroPadding(img);
    cv::Mat gray = ToGray(img2);
    cv::Mat out = MaxMin_Filter(gray, 3, false);
    cv::imshow("t13", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
