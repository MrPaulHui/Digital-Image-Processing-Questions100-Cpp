//形态学运算——腐蚀
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat erode(cv::Mat binary_, int N=1){
    cv::Mat binary = binary_.clone();
    int height = binary.rows;
    int width = binary.cols;
    for(int n=0;n<N;n++){
        cv::Mat tmp = binary.clone();
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(tmp.at<uchar>(i,j)==0)continue;
                if(j-1>=0 && tmp.at<uchar>(i,j-1)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
                if(j+1<width && tmp.at<uchar>(i,j+1)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
                if(i-1>=0 && tmp.at<uchar>(i-1,j)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
                if(i+1<height && tmp.at<uchar>(i+1,j)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
            }
        }
    }
    return binary;
}

int main(){
    cv::Mat img = cv::imread("../imgs/digit.png", cv::IMREAD_COLOR);
    cv::Mat binary = Ostu_Threshold_two(img);
    cv::imshow("binary", binary);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat out = erode(binary, 5);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
