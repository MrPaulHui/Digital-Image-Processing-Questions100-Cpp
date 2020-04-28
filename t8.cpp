// max pooling
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat MaxPool(cv::Mat img, int pool_size){
    int height = img.rows;
    int width = img.cols;
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i+=pool_size){
        for(int j=0;j<height;j+=pool_size){
            double max0 = 0;
            double max1 = 0;
            double max2 = 0;
            for(int ii=i;ii<i+pool_size;ii++){
                for(int jj=j;jj<j+pool_size;jj++){
                    max0 = fmax(max0, (double)img.at<cv::Vec3b>(ii,jj)[0]);
                    max1 = fmax(max1, (double)img.at<cv::Vec3b>(ii,jj)[1]);
                    max2 = fmax(max2, (double)img.at<cv::Vec3b>(ii,jj)[2]);
                }
            }
            for(int ii=i;ii<i+pool_size;ii++){
                for(int jj=j;jj<j+pool_size;jj++){
                    out.at<cv::Vec3b>(ii,jj)[0] = (uchar)max0;
                    out.at<cv::Vec3b>(ii,jj)[1] = (uchar)max1;
                    out.at<cv::Vec3b>(ii,jj)[2] = (uchar)max2;
                }
            }
        }
    }
    return out;
}

cv::Mat MaxPool_better(cv::Mat img, int pool_size){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i+=pool_size){
        for(int j=0;j<height;j+=pool_size){
            for(int c=0;c<channel;c++){
                double max = 0;
                for(int ii=i;ii<i+pool_size;ii++){
                    for(int jj=j;jj<j+pool_size;jj++){
                        max = fmax(max, (double)img.at<cv::Vec3b>(ii,jj)[c]);
                    }
                }
                for(int ii=i;ii<i+pool_size;ii++){
                    for(int jj=j;jj<j+pool_size;jj++){
                        out.at<cv::Vec3b>(ii,jj)[c] = (uchar)max;
                    }
                }
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = MaxPool_better(img, 8);
    cv::imshow("t8", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
