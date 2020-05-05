//仿射变换——倾斜 切变 shear
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat Shearing_Affine(cv::Mat img, double x_shear_angle, double y_shear_angle){
    int height = img.rows;
    int width = img.cols;
    double rad_x = x_shear_angle/180 * M_PI;
    double rad_y = y_shear_angle/180 * M_PI;
    double b = tan(rad_x);
    double c = tan(rad_y);
    int out_height = height + width*c;
    int out_width = width + height*b;
    return Affine(img, 1,b,c,1,0,0,out_height,out_width);
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Shearing_Affine(img, 45, 0);
    cv::imshow("t31", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
