//高斯金字塔
//真正的高斯金字塔，应该是一组之间有相同的尺度，不同的高斯模糊程度。不同组之间，有不同的尺度。
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
#include <vector>
using namespace std;

vector<cv::Mat> Guassian_Pyramid_simple(cv::Mat img, int num_scale=5){
    vector<cv::Mat> pyramids;
    cv::Mat gray = ToGray(img);
    pyramids.push_back(gray);
    cv::Mat img_pyramid = gray.clone();
    for(int i=0;i<num_scale;i++){
        img_pyramid = bilinear_interpolation_gray(img_pyramid, 0.5, 0.5);
        pyramids.push_back(img_pyramid);
    }
    return pyramids;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    int num_scale = 5;
    vector<cv::Mat> pyramids = Guassian_Pyramid_simple(img, num_scale);
    for(int i=0;i<num_scale;i++){
        cv::imshow("t74", pyramids[i]);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
}
