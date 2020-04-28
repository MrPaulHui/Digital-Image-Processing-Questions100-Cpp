//题目描述：
//读取图像，然后将$\text{RGB}$通道替换成$\text{BGR}$通道
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = img.clone();
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            unsigned char tmp = out.at<cv::Vec3b>(i,j)[0];
            out.at<cv::Vec3b>(i,j)[0] = out.at<cv::Vec3b>(i,j)[2];
            out.at<cv::Vec3b>(i,j)[2] = tmp;
        }
    }
    cv::imshow("t1", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
