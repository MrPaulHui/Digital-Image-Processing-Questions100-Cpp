#pragma once
#include <math.h>
cv::Mat ToGray(cv::Mat img){ //img的格式是bgr
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC1);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            unsigned char b = img.at<cv::Vec3b>(i,j)[0];
            unsigned char g = img.at<cv::Vec3b>(i,j)[1];
            unsigned char r = img.at<cv::Vec3b>(i,j)[2];
            out.at<unsigned char>(i,j) = 0.2126 * (double)r + 0.7152 * (double)g + 0.0722 * (double)b;
        }
    }
    return out;
}

cv::Mat ZeroPadding(cv::Mat img, int pad_size=1){
    int h = img.rows;
    int w = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(h+2*pad_size, w+2*pad_size, CV_8UC3);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            for(int c=0;c<channel;c++){
                out.at<cv::Vec3b>(i+pad_size,j+pad_size)[c] = img.at<cv::Vec3b>(i,j)[c];
            }
        }
    }
    return out;
}

cv::Mat ZeroPadding_Gray(cv::Mat img, int pad_size=1){
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h+2*pad_size, w+2*pad_size, CV_8UC1);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            out.at<uchar>(i+pad_size,j+pad_size) = img.at<uchar>(i,j);
        }
    }
    return out;
}

cv::Mat Conv_Gray(cv::Mat img, cv::Mat kernel, bool need_pad=true, bool need_clip=true){
    // stride只能为1
    int k_size = kernel.rows;
    if(need_pad){
        img = ZeroPadding_Gray(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    cv::Mat out = cv::Mat::zeros(height-k_size+1, width-k_size+1, CV_8UC1);
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            double sum = 0;
            for(int ii=0;ii<k_size;ii++){
                for(int jj=0;jj<k_size;jj++){
                    sum += kernel.at<double>(ii,jj) * (double)img.at<uchar>(i+ii,j+jj);
                }
            }
            if(need_clip){
                sum = fmax(sum, 0);
                sum = fmin(sum, 255); //需要做一手clip，保证像素值范围在0-255
            }
            out.at<uchar>(i,j) = (uchar)sum;
        }
    }
    return out;
}

cv::Mat Conv(cv::Mat img, cv::Mat kernel, bool need_pad=true, bool need_clip=false){
    // stride只能为1
    int k_size = kernel.rows;
    if(need_pad){
        img = ZeroPadding(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height-k_size+1, width-k_size+1, CV_8UC3);
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            for(int c=0;c<channel;c++){
                double sum = 0;
                for(int ii=0;ii<k_size;ii++){
                    for(int jj=0;jj<k_size;jj++){
                        sum += kernel.at<double>(ii,jj) * (double)img.at<cv::Vec3b>(i+ii,j+jj)[c];
                    }
                }
                if(need_clip){
                    sum = fmax(sum, 0);
                    sum = fmin(sum, 255); //需要做一手clip，保证像素值范围在0-255
                }
                out.at<cv::Vec3b>(i,j)[c] = (uchar)sum;
            }
        }
    }
    return out;
}
