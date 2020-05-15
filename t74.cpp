//变相的DoG边缘检测
//变相的地方在于用插值尺度变换代替了高斯滤波（都是让图像变模糊的效果）
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

cv::Mat DoG_variation(cv::Mat img){
    cv::Mat gray = ToGray(img);
    cv::Mat blur = bilinear_interpolation(bilinear_interpolation(img, 0.25, 0.25), 4, 4);
    return abs(gray-ToGray(blur));
}

cv::Mat DoG(cv::Mat img, double sigma1, double sigma2, int k_size=3){
    //似乎没有卵用诶
    cv::Mat gray = ToGray(img);
    cv::Mat G1 = Guassian_Filter(gray, sigma1, k_size);
    cv::Mat G2 = Guassian_Filter(gray, sigma2, k_size);
    return abs(G1-G2);
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = DoG_variation(img);
    cv::Mat out2 = DoG(img, 0.8, 1.3);
    cv::imshow("t74", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    double ma[2][2] = {{-1,2},{3,-5}}; 
    cv::Mat ttry(2,2,CV_64FC1,ma);
    cout<<ttry<<endl;
    cv::Mat ttrya = abs(ttry);
    cout<<ttrya<<endl;
}
