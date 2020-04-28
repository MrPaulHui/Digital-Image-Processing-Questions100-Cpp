// a test in tutorial
// 将图像的左半部分的红通道和蓝通道交换
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    int h = img.rows;
    int w = img.cols;
    for(int i=0; i<h/2; i++){
        for(int j=0; j<w/2; j++){
            // auto tmp = img.at<cv::Vec3b>(i,j)[0];
            unsigned char tmp = img.at<cv::Vec3b>(i,j)[0]; // 像素值的类型就是unsigned char
            img.at<cv::Vec3b>(i,j)[0] = img.at<cv::Vec3b>(i,j)[2];
            img.at<cv::Vec3b>(i,j)[2] = tmp;
        }
    }
    cv::imshow("test", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
