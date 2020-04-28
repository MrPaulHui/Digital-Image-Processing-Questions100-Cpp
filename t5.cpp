// HSV变换和反变换
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat RGB2HSV(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    float h, s, v;
    float r, g, b;
    float _max, _min;
    cv::Mat hsv = cv::Mat::zeros(height, width, CV_32FC3); //CV_32FC3表示每个像素值为32bit，F表示float类型，对应U表示unsigned int，C3表示3个通道
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            r = (float)img.at<cv::Vec3b>(i,j)[2]/255;
            g = (float)img.at<cv::Vec3b>(i,j)[1]/255; // 不加float类型转换，会出错，C++的类型一定要注意！！！
            b = (float)img.at<cv::Vec3b>(i,j)[0]/255; // 0:b, 1:g, 2:r
            _max = fmax(r, fmax(g, b));
            _min = fmin(r, fmin(g, b));
            // get h
            if(_max == _min){
                h = 0;
            } else if(_min == b){
                h = 60*(g-r)/(_max-_min)+60;
            } else if(_min == r){
                h = 60*(b-g)/(_max-_min)+180;
            } else if(_min == g){
                h = 60*(r-b)/(_max-_min)+300;
            }
            // get s
            s = _max - _min;
            // get v
            v = _max;
            hsv.at<cv::Vec3f>(i,j)[0] = h;
            hsv.at<cv::Vec3f>(i,j)[1] = s;
            hsv.at<cv::Vec3f>(i,j)[2] = v;
        }
    }
    return hsv;
}

cv::Mat HSV2RGB(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    float h, s, v;
    float r, g, b;
    cv::Mat rgb = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            h = img.at<cv::Vec3f>(i,j)[0];
            s = img.at<cv::Vec3f>(i,j)[1];
            v = img.at<cv::Vec3f>(i,j)[2];
            float c = s;
            float h1 = h/60;
            float x = c*(1-abs(fmod(h1,2)-1));
            r = g = b = v - c;
            if(h1<1){
                r += c;
                g += x;
            }
            else if(h1<2){
                r += x;
                g += c;
            }
            else if(h1<3){
                g += c;
                b += x;
            }
            else if(h1<4){
                g += x;
                b += c;
            }
            else if(h1<5){
                r += x;
                b += c;
            }
            else if(h1<6){
                r += c;
                b += x;
            }
            rgb.at<cv::Vec3b>(i,j)[0] = uchar(b*255);
            rgb.at<cv::Vec3b>(i,j)[1] = uchar(g*255);
            rgb.at<cv::Vec3b>(i,j)[2] = uchar(r*255);
        }
    }
    return rgb;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    int height = img.rows;
    int width = img.cols;
    cv::Mat hsv = RGB2HSV(img);
    for(int i=0;i<height;i++){
        for(int j=0;j<height;j++){
            hsv.at<cv::Vec3f>(i,j)[0] += 180; 
            hsv.at<cv::Vec3f>(i,j)[0] = fmod(hsv.at<cv::Vec3f>(i,j)[0], 360);
            // 色相反转，即给h值加180，注意要对360取模，因为h范围是0-360
        }
    }
    cv::Mat out = HSV2RGB(hsv);
    cv::imshow("t5", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
