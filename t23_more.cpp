//直方图匹配（规定化）
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat hist_match(cv::Mat src_img, cv::Mat dst_img){
    //参考：https://blog.csdn.net/hcx25909/article/details/7090921
    double* src_acc_hist = CalAccHist(hist_equalize(src_img));
    double* dst_acc_hist = CalAccHist(hist_equalize(dst_img));
    int tables[256]; //match映射表
    double min_sub;
    double sub;
    int k = 0;
    for(int i=0;i<256;i++){
        min_sub = 1000;
        for(int j=k;j<256;j++){
            sub = abs(*(src_acc_hist+i)-*(dst_acc_hist+j));
            if(sub < min_sub+1.0E-3){
                k = j;
                min_sub = sub;
            }
            else{
                k = j-1;
                break; //注意这里实现上的细节啊!!! 不能一开始就对应重复的取值！！！
            }
        }
        if(k==255){
            for(int l=i;l<256;l++){
                tables[l] = k;
            }
            break;
        }
        tables[i] = k;
    }
    for(int i=0;i<256;i++){
        cout<<tables[i]<<endl;
    }
    int height = src_img.rows;
    int width = src_img.cols;
    int channel = src_img.channels();
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                int val = (int)src_img.at<cv::Vec3b>(i,j)[_c];
                out.at<cv::Vec3b>(i,j)[_c] = (uchar)tables[val];
            }
        }
    }
    return out;
}

int main(){
    cv::Mat src_img = cv::imread("../imgs/imori_dark.jpg", cv::IMREAD_COLOR);
    cv::Mat dst_img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    Hist(src_img);
    Hist(dst_img);
    cv::Mat out = hist_match(src_img, dst_img);
    cv::imshow("t23_more", out);
    cv::waitKey(0);
    Hist(out);
    cv::destroyAllWindows();
}
