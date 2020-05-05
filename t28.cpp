//仿射变换——平移
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace std;

bool val_in(int x, int min, int max){
    if(x>=min && x<=max){
        return true;
    }
    else{
        return false;
    }
}

cv::Mat Affine(cv::Mat img, double a, double b, double c, double d, double tx, double ty){
    //a,b,c,d,tx,ty为仿射变换矩阵的参数
    /*
    变换矩阵：Aff=
    [[a, b, tx],
     [c, d, ty],
     [0, 0, 1]]
    原始坐标 P = (x, y, 1)^T
    变换后坐标 P' = (x', y', 1)^T
    P' = Aff * P
    实现上用反变换，即P = Aff^-1 * P'
    */
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    int ori_i, ori_j;
    int out_height = height * d;
    int out_width = width * a;
    double det = a*d - b*c;
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC3);
    for(int i=0;i<out_height;i++){
        for(int j=0;j<out_width;j++){
            ori_i = (-c*j + a*i)/det - ty;
            ori_j = (d*j - b*i)/det - tx;
            if(val_in(ori_i, 0, height-1) && val_in(ori_j, 0, width-1)){
                for(int c=0;c<channel;c++){
                    out.at<cv::Vec3b>(i, j)[c] = img.at<cv::Vec3b>(ori_i, ori_j)[c];
                }
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Affine(img, 1,0,0,1,30,-30);
    cv::imshow("t28", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
