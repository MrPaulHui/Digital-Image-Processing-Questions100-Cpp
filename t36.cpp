//DCT 离散余弦变换
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <stdexcept>
using namespace std;

cv::Mat DCT(cv::Mat img, cv::Mat* A_pointer=nullptr){
    int height = img.rows;
    int width = img.cols;
    if(height != width){
        throw runtime_error("the height must equal to width");
    }
    int channel = img.channels();
    cv::Mat A;
    if(A_pointer==nullptr){
        A = cv::Mat::zeros(height, width, CV_64FC1);
        double coff;
        for(int u=0;u<height;u++){
            for(int x=0;x<height;x++){
                if(u==0){
                    coff = sqrt(1/(double)height);
                }
                else{
                    coff = sqrt(2/(double)height);
                }
                A.at<double>(u,x) = coff*cos((x+0.5)*M_PI*(double)u/(double)height);
            }
        }
    }
    else{
        A = *A_pointer;
    }
    cv::Mat img_HD;
    img.convertTo(img_HD, CV_64F);
    cv::Mat out;
    if(channel==3){
        vector<cv::Mat> imgs(3);
        cv::split(img_HD, imgs);
        vector<cv::Mat> dcts;
        for(int i=0;i<3;i++){
            dcts.push_back(A*imgs[i]*(A.t()));
        }
        cv::merge(dcts, out);
    }
    else{
        out = A*img_HD*(A.t());
    }
    return out;
}

cv::Mat IDCT(cv::Mat dct, int want_K=0, cv::Mat* A_pointer=nullptr){
    int height = dct.rows;
    int width = dct.cols;
    int channel = dct.channels();
    cv::Mat A;
    if(A_pointer==nullptr){
        A = cv::Mat::zeros(height, width, CV_64FC1);
        int K;
        if(want_K<=0 || want_K>height){
            K = height;
        }
        else{
            K = want_K;
        }
        double coff;
        for(int u=0;u<K;u++){
            for(int x=0;x<height;x++){
                if(u==0){
                    coff = sqrt(1/(double)height);
                }
                else{
                    coff = sqrt(2/(double)height);
                }
                A.at<double>(u,x) = coff*cos((x+0.5)*M_PI*(double)u/(double)height);
            }
        }
    }
    else{
        A = *A_pointer;
    }
    cv::Mat img;
    if(channel==3){
        vector<cv::Mat> dcts(3);
        cv::split(dct, dcts);
        vector<cv::Mat> imgs;
        for(int i=0;i<3;i++){
            imgs.push_back((A.t())*dcts[i]*A);
        }
        cv::merge(imgs, img);
    }
    else{
        img = (A.t())*dct*A;
    }
    cv::Mat out;
    img.convertTo(out, CV_8U);
    return out;
}

cv::Mat DCT_JPEG(cv::Mat img, int block_size=8){
    //do 8*8 dct on a total img
    int height = img.rows;
    int width = img.cols;
    cv::Mat A = cv::Mat::zeros(block_size, block_size, CV_64FC1);
    double coff;
    for(int u=0;u<block_size;u++){
        for(int x=0;x<block_size;x++){
            if(u==0){
                coff = sqrt(1/(double)block_size);
            }
            else{
                coff = sqrt(2/(double)block_size);
            }
            A.at<double>(u,x) = coff*cos((x+0.5)*M_PI*(double)u/(double)block_size);
        }
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_64FC3);
    cv::Mat block, block_dct;
    for(int i=0;i<height;i+=block_size){
        for(int j=0;j<width;j+=block_size){
            cv::Rect rect(i,j,block_size,block_size);
            block = cv::Mat(img, rect);
            block_dct = DCT(block, &A);
            block_dct.copyTo(out(rect));
        }
    }
    return out;
}

cv::Mat IDCT_JPEG(cv::Mat dct, int want_K=0, int block_size=8){
    //do 8*8 idct on a total img
    int height = dct.rows;
    int width = dct.cols;
    cv::Mat A = cv::Mat::zeros(block_size, block_size, CV_64FC1);
    int K;
    if(want_K<=0 || want_K>block_size){
        K = block_size;
    }
    else{
        K = want_K;
    }
    double coff;
    for(int u=0;u<K;u++){
        for(int x=0;x<block_size;x++){
            if(u==0){
                coff = sqrt(1/(double)block_size);
            }
            else{
                coff = sqrt(2/(double)block_size);
            }
            A.at<double>(u,x) = coff*cos((x+0.5)*M_PI*(double)u/(double)block_size);
        }
    }
    cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
    cv::Mat block, block_dct;
    for(int i=0;i<height;i+=block_size){
        for(int j=0;j<width;j+=block_size){
            cv::Rect rect(i,j,block_size,block_size);
            block_dct = cv::Mat(dct, rect);
            block = IDCT(block_dct, K, &A);
            block.copyTo(img(rect));
        }
    }
    return img;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    //cv::Mat dct = DCT_JPEG(img);
    //cv::Mat oimg = IDCT_JPEG(dct,1,8);
    cv::Mat dct = DCT(img);
    cv::Mat oimg = IDCT(dct, 90, nullptr);
    cv::imshow("t36", oimg);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
