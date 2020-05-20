//PSNR
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

double PSNR(cv::Mat test_img, cv::Mat standard_img){
    int height = test_img.rows;
    int width = test_img.cols;
    int channel = test_img.channels();
    double mse = 0;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int c=0;c<channel;c++){
                mse += pow((double)test_img.at<cv::Vec3b>(i,j)[c]-(double)standard_img.at<cv::Vec3b>(i,j)[c], 2);
            }
        }
    }
    mse /= (height*width*channel);
    double psnr = 10*log10(255.0*255.0/mse);
    return psnr;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat img0 = IDCT(DCT(img),110, nullptr);
    cv::Mat img1 = IDCT(DCT(img), 70, nullptr);
    cv::Mat img2 = IDCT(DCT(img), 20, nullptr);
    double psnr0 = PSNR(img0, img);
    double psnr1 = PSNR(img1, img);
    double psnr2 = PSNR(img2, img);
    cout<<"PSNR0="<<psnr0<<endl;
    cv::imshow("img0", img0);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cout<<"PSNR1="<<psnr1<<endl;
    cv::imshow("img1", img1);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cout<<"PSNR2="<<psnr2<<endl;
    cv::imshow("img2", img2);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
