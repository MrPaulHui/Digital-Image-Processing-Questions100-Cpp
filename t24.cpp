//gamma correction
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <iostream>
using namespace std;

cv::Mat gamma_correction(cv::Mat img, double gamma, double c=1){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                double val = (double)img.at<cv::Vec3b>(i,j)[_c] / 255.0; //这里先做归一化
                out.at<cv::Vec3b>(i,j)[_c] = 1/c * pow(val, 1/gamma) * 255; //乘255恢复到0-255区间
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_gamma.jpg", cv::IMREAD_COLOR);
    cv::Mat out = gamma_correction(img, 2.2);
    cv::imshow("t24", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
