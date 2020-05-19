//Discrete Fourier Transformation DFT
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat* DFT(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    if(channel==3){
        img = ToGray(img);
    }
    cv::Mat real = cv::Mat::zeros(height, width, CV_64FC1);
    cv::Mat imag = cv::Mat::zeros(height, width, CV_64FC1);
    double I, angle;
    double real_val, imag_val;
    for(int l=0;l<height;l++){
        for(int k=0;k<width;k++){
            real_val = 0;
            imag_val = 0;
            for(int y=0;y<height;y++){
                for(int x=0;x<width;x++){
                    I = (double)img.at<uchar>(y, x);
                    angle = -2 * M_PI *(double(k)*double(x)/double(width)+double(l)*double(y)/double(height));
                    real_val += I*cos(angle);
                    imag_val += I*sin(angle);
                }
            }
            real_val /= sqrt(height*width);
            imag_val /= sqrt(height*width);
            //cout<<"real "<<real_val<<" imag "<<imag_val<<endl;
            real.at<double>(l,k) = real_val;
            imag.at<double>(l,k) = imag_val;
        }
    }
    static cv::Mat fourier_result[2];
    fourier_result[0] = real;
    fourier_result[1] = imag;
    return fourier_result;
}

cv::Mat* DFT_advanced(cv::Mat img){
    //速度秒杀原始版本，原理见https://www.jianshu.com/p/98f493de01db
    //这里是把实部和虚部分开计算，IDFT的advanced版本同理，矩阵拆开相乘再相加
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    if(channel==3){
        img = ToGray(img);
    }
    cv::Mat real;
    cv::Mat imag;
    cv::Mat G11 = cv::Mat::zeros(height, height, CV_64FC1);
    cv::Mat G12 = cv::Mat::zeros(height, height, CV_64FC1);
    cv::Mat G21 = cv::Mat::zeros(width, width, CV_64FC1);
    cv::Mat G22 = cv::Mat::zeros(width, width, CV_64FC1);
    double angle;
    for(int i=0;i<height;i++){
        for(int j=0;j<height;j++){
            angle = -2 * M_PI * (double)i*(double)j/(double)height;
            G11.at<double>(i,j) = cos(angle);
            G12.at<double>(i,j) = sin(angle);
        }
    }
    for(int i=0;i<width;i++){
        for(int j=0;j<width;j++){
            angle = -2 * M_PI * (double)i*(double)j/(double)width;
            G21.at<double>(i,j) = cos(angle);
            G22.at<double>(i,j) = sin(angle);
        }
    }
    cv::Mat I;
    img.convertTo(I, CV_64F);
    real = G11*I*G21-G12*I*G22;
    imag = G11*I*G22+G12*I*G21;
    real = real/sqrt(height*width);
    imag = imag/sqrt(height*width);
    static cv::Mat fourier_result[2];
    fourier_result[0] = real;
    fourier_result[1] = imag;
    return fourier_result;
}

cv::Mat get_DFT_magnitude(cv::Mat* fourier_result){
    cv::Mat magnitude = (*fourier_result).mul(*fourier_result)+(*(fourier_result+1)).mul(*(fourier_result+1));
    for(int i=0;i<magnitude.rows;i++){
        for(int j=0;j<magnitude.cols;j++){
            magnitude.at<double>(i,j) = sqrt(magnitude.at<double>(i,j));
        }
    }
    return magnitude;
}

cv::Mat IDFT(cv::Mat* fourier_result){
    cv::Mat real = *fourier_result;
    cv::Mat imag = *(fourier_result+1);
    int height = real.rows;
    int width = real.cols;
    cv::Mat img = cv::Mat::zeros(height, width, CV_8UC1);
    double G_real, G_imag, angle;
    double img_real, img_imag;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            img_real = 0;
            img_imag = 0;
            for(int l=0;l<height;l++){
                for(int k=0;k<width;k++){
                    G_real = real.at<double>(l,k);
                    G_imag = imag.at<double>(l,k);
                    angle = 2 * M_PI *(double(k)*double(x)/double(width)+double(l)*double(y)/double(height));
                    img_real += G_real*cos(angle)-G_imag*sin(angle);
                    img_imag += G_real*sin(angle)+G_imag*cos(angle);
                }
            }
            img.at<uchar>(y,x) = (uchar)(sqrt(img_real*img_real+img_imag*img_imag)/sqrt(height*width));
        }
    }
    return img;
}

cv::Mat IDFT_advanced(cv::Mat* fourier_result){
    cv::Mat real = *fourier_result;
    cv::Mat imag = *(fourier_result+1);
    int height = real.rows;
    int width = real.cols;
    cv::Mat img;
    cv::Mat img_real, img_imag;
    cv::Mat G31 = cv::Mat::zeros(height, height, CV_64FC1);
    cv::Mat G32 = cv::Mat::zeros(height, height, CV_64FC1);
    cv::Mat G41 = cv::Mat::zeros(width, width, CV_64FC1);
    cv::Mat G42 = cv::Mat::zeros(width, width, CV_64FC1);
    double angle;
    for(int i=0;i<height;i++){
        for(int j=0;j<height;j++){
            angle = 2 * M_PI * (double)i*(double)j/(double)height;
            G31.at<double>(i,j) = cos(angle);
            G32.at<double>(i,j) = sin(angle);
        }
    }
    for(int i=0;i<width;i++){
        for(int j=0;j<width;j++){
            angle = 2 * M_PI * (double)i*(double)j/(double)width;
            G41.at<double>(i,j) = cos(angle);
            G42.at<double>(i,j) = sin(angle);
        }
    }
    img_real = G31*real*G41-G31*imag*G42-G32*real*G42-G32*imag*G41;
    img_imag = G31*real*G42+G31*imag*G41+G32*real*G41-G32*imag*G42;
    img = img_real.mul(img_real) + img_imag.mul(img_imag);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            img.at<double>(i,j) = sqrt(img.at<double>(i,j))/sqrt(height*width);
        }
    }
    cv::Mat img_;
    img.convertTo(img_, CV_8U);
    return img_;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat* fourier = DFT_advanced(img);
    cv::Mat oimg = IDFT_advanced(fourier);
    cv::Mat magnit = get_DFT_magnitude(fourier);
    double minval, maxval;
    cv::minMaxIdx(magnit, &minval, &maxval);
    magnit = magnit/maxval*255;
    cv::Mat disp;
    magnit.convertTo(disp, CV_8U);
    cv::imshow("magnitude", disp);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imshow("oimg", oimg);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
