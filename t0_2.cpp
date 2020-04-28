// 像素级操作，循环写法，拼接图像
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR); //note C++里面用双引号！！！！！
    cv::Mat img2 = img.clone();
    for(int i=0; i<img2.rows; i++){
        for(int j=0; j<img2.cols; j++){
            img2.at<cv::Vec3b>(i,j)[0] += 1;
            img2.at<cv::Vec3b>(i,j)[1] += 2;
            img2.at<cv::Vec3b>(i,j)[2] += 3;
        }
    }
    cv::Mat disp;
    cv::Mat img_list[2];
    img_list[0] = img;
    img_list[1] = img2;
    cv::hconcat(img_list, 2, disp);
    cv::imshow("concat", disp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
