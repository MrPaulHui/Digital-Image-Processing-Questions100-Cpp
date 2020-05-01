//LoG边缘检测，相比Lapalacian，减少了噪声异常点的影响 如果直接用Lapalacian边缘检测带明显噪声的图像，那么效果会爆炸的，可以试一下t17检测noise图像。
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "common.h"
using namespace std;

cv::Mat get_LoG_kernel(double sigma, int k_size){
    double kernel[k_size][k_size];
    double pad = floor(k_size/2);
    int _x = 0, _y = 0;
    double kernel_sum = 0;
    for (int y = 0; y < k_size; y++){
        for (int x = 0; x < k_size; x++){
            _y = y - pad;
            _x = x - pad; 
            kernel[y][x] = (_x*_x+_y*_y-sigma*sigma) / (2 * M_PI * pow(sigma, 6)) * exp( - (_x * _x + _y * _y) / (2 * sigma * sigma));
            kernel_sum += kernel[y][x];
        }
    }
    for (int y = 0; y < k_size; y++){
        for (int x = 0; x < k_size; x++){
            kernel[y][x] /= kernel_sum; //归一化
        }
    }
    cv::Mat kernel_mat(k_size, k_size, CV_64FC1, kernel); //直接返回kernel_mat会出问题，用下面两种方法可以解决
    /*
    cv::Mat kernel_mat = cv::Mat::zeros(k_size, k_size, CV_64FC1);
    for (int y = 0; y < k_size; y++){
        for (int x = 0; x < k_size; x++){
            kernel_mat.at<double>(y,x) = kernel[y][x];
        }
    }
    return kernel_mat;
    */
    cv::Mat out = kernel_mat.clone();
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_noise.jpg", cv::IMREAD_COLOR);
    cv::Mat gray = ToGray(img);
    cv::Mat LoG_kernel = get_LoG_kernel(3, 5);
    cv::Mat out = Conv_Gray(gray, LoG_kernel);
    cv::imshow("t19", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
