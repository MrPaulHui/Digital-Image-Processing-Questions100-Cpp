//题目描述：
//大津二值化算法
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

cv::Mat ToGray(cv::Mat img){
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC1);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            unsigned char r = img.at<cv::Vec3b>(i,j)[0];
            unsigned char g = img.at<cv::Vec3b>(i,j)[1];
            unsigned char b = img.at<cv::Vec3b>(i,j)[2]; //尖括号似乎对应着模板。。
            out.at<unsigned char>(i,j) = 0.2126 * r + 0.7152 * g + 0.0722 * b; //注意这里at后面尖括号的类型，很重要，这个类型是由访问到的元素的类型所决定的。
            // uchar = unsigned char
        }
    }
    return out;
}

cv::Mat Ostu_Threshold_two(cv::Mat img){
    cv::Mat gray = ToGray(img);
    int height = gray.rows;
    int width = gray.cols;
    double w0=0, w1=0, u0=0, u1=0, max_g=0;
    int th;
    for(int t=0; t<255; t++){
        w0=0;
        w1=0;
        u0=0;
        u1=0;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                int value = int(gray.at<uchar>(i, j));
                if(value<t){
                    w0++;
                    u0 += value;
                }
                else{
                    w1++;
                    u1 += value;
                }
            }
        }
        u0 /= w0;
        cout<<u0<<" "<<w0<<endl;
        u1 /= w1;
        w0 /= (height*width);
        w1 /= (height*width);
        double g = w0 * w1 * pow((u0-u1), 2);
        if(g>max_g){
            max_g=g;
            th = t;
        }
    }
    cout<<"threshold is: "<<th<<endl;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(gray.at<uchar>(i,j)<=th){
                gray.at<uchar>(i,j) = 0;
            }
            else{
                gray.at<uchar>(i,j) = 255;
            }
        }
    }
    return gray;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Ostu_Threshold_two(img);
    cv::imshow("t3", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
