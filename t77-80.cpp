//Gabor滤波
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat get_gabor_kernel(int K_size, double gamma, double sigma, double lambda, double phase, double angle){
    double gabor_kernel[K_size][K_size];
    int pad = floor(K_size/2);
    int px, py;
    double _x, _y;
    double rad = angle/180.0*M_PI;
    double kernel_sum=0;
    for(int y=0;y<K_size;y++){
        for(int x=0;x<K_size;x++){
            py = y-pad;
            px = x-pad;
            _y = -sin(rad)*px+cos(rad)*py;
            _x = cos(rad)*px+sin(rad)*py;
            gabor_kernel[y][x] = exp(-(_x*_x+gamma*gamma*_y*_y)/(2*sigma*sigma))*cos(2*M_PI*_x/lambda+phase);
            kernel_sum += fabs(gabor_kernel[y][x]);
        }
    }
    for (int y = 0; y < K_size; y++){
        for (int x = 0; x < K_size; x++){
            gabor_kernel[y][x] /= kernel_sum; //归一化
        }
    }
    cv::Mat kernel_mat(K_size, K_size, CV_64FC1, gabor_kernel); //直接返回kernel_mat会出问题，用下面方法可以解决
    cv::Mat out = kernel_mat.clone();
    return out;
}

void Vis_HDImg(cv::Mat img_hd, bool acc_norm=false){
    //vis img of CV_64F
    cv::Mat img = img_hd.clone();
    //normalize
    double minval, maxval;
    cv::minMaxIdx(img, &minval, &maxval);
    if(acc_norm) img = (img - minval)/(maxval - minval)*255;
    else img = img/maxval*255;
    cv::Mat disp;
    img.convertTo(disp, CV_8U);
    cv::imshow("disp", disp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

cv::Mat Gabor_edge_detection(cv::Mat img, cv::Mat gabor_filter){
    cv::Mat gray = ToGray(img);
    cv::Mat edge = Conv_Gray_HD(gray, gabor_filter);
    return edge;
}

cv::Mat Gabor_feature(cv::Mat img, int K_size, double gamma, double sigma, double lambda, double phase, double* angle, int angle_num){
    //gabor滤波器提取特征，即把不同角度检测得到的边缘叠加起来，构成特征。据说CNN的效果很像这个2333
    cv::Mat feature = cv::Mat::zeros(img.rows, img.cols, CV_64FC1);
    cv::Mat gabor, edge;
    for(int i=0;i<angle_num;i++){
        gabor = get_gabor_kernel(K_size, gamma, sigma, lambda, phase, *(angle+i));
        edge = Gabor_edge_detection(img, gabor);
        feature = feature+edge;
    }
    return feature;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    //t77-78 vis gabor filter with different angle
    /*
    for(int i=0;i<4;i++){
        cv::Mat gabor = get_gabor_kernel(111, 1.2, 10, 10, 0, 45*i);
        Vis_HDImg(gabor, true);
    }
    */
    //t79 edge detection with different angle of filter
/*
    for(int i=0;i<4;i++){
        cv::Mat gabor = get_gabor_kernel(11, 1.2, 1.5, 3, 0, 45*i);
        cv::Mat edge = Gabor_edge_detection(img, gabor);
        Vis_HDImg(edge);
    }
*/
   //t80 get feature using gabor
   double angles[4] = {0,45,90,135};
   cv::Mat feat = Gabor_feature(img, 11, 1.2, 1.5, 3, 0, angles, 4);
   Vis_HDImg(feat);
}
