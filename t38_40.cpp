//JPEG压缩
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <stdexcept>
using namespace std;

cv::Mat JPEGCompress(cv::Mat img){
    //其实个人感觉不用ycbcr空间的效果更好诶
    //step 1. bgr to ycbcr
    cv::Mat ycbcr = BGR2YCbCr(img);
    //step 2. do DCT on ycbcr with block 8*8
    cv::Mat dct = DCT_JPEG(ycbcr);
    //step 3. do quantitation on dct
    // Q table for Y
    double Q1[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                    {12, 12, 14, 19, 26, 58, 60, 55},
                    {12, 12, 14, 19, 26, 58, 60, 55},
                    {14, 17, 22, 29, 51, 87, 80, 62},
                    {18, 22, 37, 56, 68, 109, 103, 77},
                    {24, 35, 55, 64, 81, 104, 113, 92},
                    {49, 64, 78, 87, 103, 121, 120, 101},
                    {72, 92, 95, 98, 112, 100, 103, 99}
                  };

  // Q table for Cb Cr
    double Q2[8][8] = {{17, 18, 24, 47, 99, 99, 99, 99},
                    {18, 21, 26, 66, 99, 99, 99, 99},
                    {24, 26, 56, 99, 99, 99, 99, 99},
                    {47, 66, 99, 99, 99, 99, 99, 99},
                    {99, 99, 99, 99, 99, 99, 99, 99},
                    {99, 99, 99, 99, 99, 99, 99, 99},
                    {99, 99, 99, 99, 99, 99, 99, 99},
                    {99, 99, 99, 99, 99, 99, 99, 99}
                  };
    for(int i=0;i<dct.rows;i+=8){
        for(int j=0;j<dct.cols;j+=8){
            for(int ii=0;ii<8;ii++){
                for(int jj=0;jj<8;jj++){
                    dct.at<cv::Vec3d>(i+ii,j+jj)[0] = round(dct.at<cv::Vec3d>(i+ii,j+jj)[0]/Q1[ii][jj])*Q1[ii][jj];
                    dct.at<cv::Vec3d>(i+ii,j+jj)[1] = round(dct.at<cv::Vec3d>(i+ii,j+jj)[1]/Q2[ii][jj])*Q2[ii][jj];
                    dct.at<cv::Vec3d>(i+ii,j+jj)[2] = round(dct.at<cv::Vec3d>(i+ii,j+jj)[2]/Q2[ii][jj])*Q2[ii][jj];
                }
            }
        }
    }
    //step 4. do IDCT
    cv::Mat new_ycbcr = IDCT_JPEG(dct,0,8,false);
    //step 5. ycbcr to bgr
    cv::Mat new_img = YCbCr2BGR(new_ycbcr);
    return new_img;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat new_img = JPEGCompress(img);
    double psnr = PSNR(new_img, img);
    cout<<psnr<<endl;
    cv::imwrite("../img_result/after_jpeg.jpg", new_img);
    cv::imshow("new_img", new_img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
