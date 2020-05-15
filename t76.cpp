//显著图 saliency map
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
#include <vector>
#include <math.h>
using namespace std;

vector<cv::Mat> Guassian_Pyramid_simple_for_saliencyMap(cv::Mat img, int num_scale=5){
    vector<cv::Mat> pyramids;
    cv::Mat gray = ToGray(img);
    cv::Mat gray_HD;
    gray.convertTo(gray_HD, CV_64F);
    pyramids.push_back(gray_HD);
    cv::Mat img_pyramid;
    cv::Mat img_pyramid_HD;
    for(int i=1;i<=num_scale;i++){
        img_pyramid = bilinear_interpolation_gray(gray, 1.0/pow(2,i), 1.0/pow(2,i));
        img_pyramid = bilinear_interpolation_gray(img_pyramid, pow(2,i), pow(2,i));
        img_pyramid.convertTo(img_pyramid_HD, CV_64F);
        pyramids.push_back(img_pyramid_HD);
        cout<<img_pyramid_HD.size()<<endl;
    }
    return pyramids;
}

cv::Mat saliency_map(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    vector<cv::Mat> pyramids = Guassian_Pyramid_simple_for_saliencyMap(img);
    cv::Mat map = cv::Mat::zeros(height, width, CV_64FC1);
    map += abs(pyramids[0]-pyramids[1]);
    map += abs(pyramids[0]-pyramids[3]);
    map += abs(pyramids[0]-pyramids[5]);
    map += abs(pyramids[1]-pyramids[4]);
    map += abs(pyramids[2]-pyramids[3]);
    map += abs(pyramids[3]-pyramids[5]);
    double minval, maxval;
    cv::minMaxIdx(map, &minval, &maxval);
    cout<<maxval<<endl;
    map = map/maxval*255; //嘿，你别说，C++的opencv还真能这么操作
    cv::Mat map_;
    map.convertTo(map_,CV_8U); //要转换为8U，才能正常显示输出哦
    return map_;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat map = saliency_map(img);
    cv::imshow("t74", map);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
