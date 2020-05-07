//canny边缘检测
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat Canny(cv::Mat img, double high_thresh=100, double low_thresh=20, bool disp_middle=false){
    int height = img.rows;
    int width = img.cols;
    //step 1. to gray
    cv::Mat grays = ToGray(img);
    //step 2. guassian filter
    cv::Mat gray = Guassian_Filter(grays, 1.4, 5);
    //step 3. sobel filter to get gradient fx and fy
    double sobel_filter_row[3][3] = {
        {-1.0, -2.0, -1.0},
        {0, 0, 0},
        {1.0, 2.0, 1.0},
    };
    cv::Mat sobel_kernel_row(3,3,CV_64FC1,sobel_filter_row);
    double sobel_filter_col[3][3] = {
        {-1.0, 0, 1.0},
        {-2.0, 0, 2.0},
        {-1.0, 0, 1.0},
    };
    cv::Mat sobel_kernel_col(3,3,CV_64FC1,sobel_filter_col);
    cv::Mat grad_row = Conv_Gray_HD(gray, sobel_kernel_row); //算梯度应定要用高精度卷积，要不然很影响下面幅度和角度的计算
    cv::Mat grad_col = Conv_Gray_HD(gray, sobel_kernel_col);
    //step 4. get grad magnitude and direction
    //cv::Mat grad_magnitude = Clip(out_row + out_col); //这一步是近似运算
    cv::Mat grad_magnitude = cv::Mat::zeros(height, width, CV_8UC1); 
    //这一步数据类型存疑啊，uchar是不是精度太低了，保存的可是梯度啊，包括conv函数里的似乎也有这个问题.
    //ps:解决了哈哈，确实有这个问题，conv加大精度之后解决了,不过这里就用8精度
    cv::Mat grad_direction = cv::Mat::zeros(height, width, CV_8UC1);
    double grad_y, grad_x, grad_m, grad_d, angle;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            grad_y = grad_row.at<double>(i,j);
            grad_x = grad_col.at<double>(i,j);
            //step 4.1. get magnitude
            grad_m = sqrt(grad_y*grad_y+grad_x*grad_x);
            grad_magnitude.at<uchar>(i,j) = (uchar)clip_val(grad_m, 0, 255);
            //step 4.2. get and quantizaiton direction angle
            grad_d = atan2(grad_y, grad_x); //返回值是弧度值
            angle = grad_d / M_PI * 180;
            if(angle < -22.5){
                angle = 180 + angle;
            }
            else if(angle >= 157.5){
                angle = angle - 180;
            } //将角度值范围控制在-22.5_157.5
            //quantizaiton
            if (angle <= 22.5){
                grad_direction.at<uchar>(i,j) = 0;
            } else if (angle <= 67.5){
                grad_direction.at<uchar>(i,j) = 45;
            } else if (angle <= 112.5){
                grad_direction.at<uchar>(i,j) = 90;
            } else {
                grad_direction.at<uchar>(i,j) = 135;
            }
        }
    }
    if(disp_middle){
        cv::imshow("grad_magnitude", grad_magnitude);
        cv::waitKey(0);
        cv::destroyAllWindows();
        cv::imshow("grad_direction", grad_direction);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 5. nms to make edge thin
    cv::Mat edge_thin = cv::Mat::zeros(height, width, CV_8UC1);
    double direction, magnit;
    int dx1, dy1, dx2, dy2;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            magnit = grad_magnitude.at<uchar>(i,j);
            direction = grad_direction.at<uchar>(i,j);
            if(direction==0){
                dx1 = -1;
                dy1 = 0;
                dx2 = 1;
                dy2 = 0;
            }
            if(direction==45){
                dx1 = -1;
                dy1 = -1;
                dx2 = 1;
                dy2 = 1;
            }
            if(direction==90){
                dx1 = 0;
                dy1 = -1;
                dx2 = 0;
                dy2 = 1;
            }
            if(direction==135){
                dx1 = -1;
                dy1 = 1;
                dx2 = 1;
                dy2 = -1;
            }
            //考虑边界情况
            if (j == 0){
                dx1 = fmax(dx1, 0);
                dx2 = fmax(dx2, 0);
            }
            if (j == (width - 1)){
                dx1 = fmin(dx1, 0);
                dx2 = fmin(dx2, 0);
            }
            if (i == 0){
                dy1 = fmax(dy1, 0);
                dy2 = fmax(dy2, 0);
            }
            if (i == (height - 1)){
                dy1 = fmin(dy1, 0);
                dy2 = fmin(dy2, 0);
            }
            if(magnit >= grad_magnitude.at<uchar>(i+dy1,j+dx1) && magnit >= grad_magnitude.at<uchar>(i+dy2,j+dx2)){
                edge_thin.at<uchar>(i,j) = (uchar)magnit;
            }
        }
    }
    if(disp_middle){
        cv::imshow("edge_thin", edge_thin);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 6. using two thresholds to binarize
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);
    double val;
    bool flag;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            val = (double)edge_thin.at<uchar>(i,j);
            if(val>=high_thresh){
                out.at<uchar>(i,j) = 255;
            }
            else if(val<=low_thresh){
                continue;
            }
            else{
                flag = false;
                for(int dyy=-1;dyy<2;dyy++){
                    for(int dxx=-1;dxx<2;dxx++){
                        if(dyy==0 && dxx==0) continue;
                        if(val_in(i+dyy,0,height-1)==false || val_in(j+dxx,0,width-1)==false) continue;
                        if((double)edge_thin.at<uchar>(i+dyy,j+dxx)>=high_thresh){
                            flag = true;
                            break;
                        }
                    }
                    if(flag) break;
                }
                if(flag){
                    out.at<uchar>(i,j) = 255;
                }
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/thorino.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Canny(img, 100, 30, true);
    cv::imshow("canny", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    //cout<<atan2(-1,1)/M_PI*180<<endl;
    //cout<<atan2(1,0)/M_PI*180<<endl;
}
