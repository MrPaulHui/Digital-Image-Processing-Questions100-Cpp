//双三次插值
//因为用到了周围16个像素，所以要判断像素位置是否超出
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

double h(double t, double a=-1){
    if(t<=1){
        return (a+2)*pow(t,3) - (a+3)*t*t + 1;
    }
    else if(t<=2){
        return a*pow(t,3) - 5*a*t*t + 8*a*t -4*a;
    }
    else{
        return 0;
    }
}

cv::Mat bicubic_interpolation(cv::Mat img, double scale){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    int out_height = (int)(height*scale);
    int out_width = (int)(width*scale);
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC3);
    int x, y;
    double dx[4], dy[4];
    double val;
    double weight_sum;
    double hk, hm;
    int _x, _y;
    for(int i=0;i<out_height;i++){
        y = (int)floor(i/scale);
        for(int k=0;k<4;k++){
            dy[k] = abs(i/scale-clip_val(y+k-1, 0, height-1)); //防止位置超出图像范围
        }
        for(int j=0;j<out_width;j++){
            x = (int)floor(j/scale);
            for(int k=0;k<4;k++){
                dx[k] = abs(j/scale-clip_val(x+k-1, 0, width-1));
            }
            for(int c=0;c<channel;c++){
                val = 0;
                weight_sum=0;
                for(int k=0;k<4;k++){
                    hk = h(dy[k]);
                    _y = clip_val(y+k-1, 0, height-1); //防止位置超出图像范围
                    for(int m=0;m<4;m++){
                        hm = h(dx[m]);
                        weight_sum += hk*hm;
                        _x = clip_val(x+m-1, 0, width-1);
                        val += (double)img.at<cv::Vec3b>(_y,_x)[c]*hk*hm;
                    }
                }
                val /= weight_sum;
                val = clip_val(val, 0, 255); //妈的，问题出在没有加这一步
                out.at<cv::Vec3b>(i,j)[c] = (uchar)val;
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = bicubic_interpolation(img, 6);
    cv::imshow("t27", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
