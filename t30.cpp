//仿射变换——旋转
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat Rotate_Affine(cv::Mat img, double angle, bool rotate_center=false, bool disp_all=false){
    //参数值的详细推导过程见note，很重要！
    //rotate params
    double a, b, c, d;
    double rad = angle/180 * M_PI;
    a = cos(rad);
    b = -sin(rad);
    c = sin(rad);
    d = cos(rad);
    int height = img.rows;
    int width = img.cols;
    int out_height = height;
    int out_width = width;
    double tx = 0;
    double ty = 0;
    if(rotate_center){
        tx = (width*cos(rad)-height*sin(-rad)-width)/2;
        ty = (width*sin(-rad)+height*cos(rad)-height)/2;
        if(disp_all){
            out_height = height*abs(cos(rad)) + width*abs(sin(rad));
            out_width = height*abs(sin(rad)) + width*abs(cos(rad));
            tx = (out_width*cos(rad)-out_height*sin(-rad)-width)/2;
            ty = (out_width*sin(-rad)+out_height*cos(rad)-height)/2;
        }
    }
    cout<<out_height<<endl<<out_width<<endl;
    return Affine(img, a,b,c,d,tx,ty,out_height,out_width);
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);

    cv::Mat out = Rotate_Affine(img, -30, true, false);
    cout<<"out_shape: "<<out.rows<<" "<<out.cols<<endl;
    cv::imshow("t29", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    /*
    cv::Mat img1 = Affine(img, 1,0,0,1,-128.0/2,-128.0/2);
    cv::imshow("t29", img1);
    cv::waitKey(0);
    cv::Mat img2 = Affine(img1, 0.866,0.5,-0.5,0.866,0,0);
    cv::imshow("t29", img2);
    cv::waitKey(0);
    cv::Mat img3 = Affine(img2, 1,0,0,1,128.0/2,128.0/2);
    cv::imshow("t29", img3);
    cv::waitKey(0);
    cv::destroyAllWindows();
    */
}
