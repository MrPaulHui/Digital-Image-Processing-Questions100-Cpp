//题目描述：
//读取图像，然后将$\text{RGB}$通道替换成$\text{BGR}$通道
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;

cv::Mat swap_channel(cv::Mat img){
    int h = img.rows;
    int w = img.rows;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC3);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            out.at<cv::Vec3b>(i,j)[0] = img.at<cv::Vec3b>(i,j)[2];
            out.at<cv::Vec3b>(i,j)[1] = img.at<cv::Vec3b>(i,j)[1];
            out.at<cv::Vec3b>(i,j)[2] = img.at<cv::Vec3b>(i,j)[0];
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = swap_channel(img);
    cv::imshow("t1", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
