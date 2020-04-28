// 减色处理
// rgb根据范围只用四个值表示
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;

uchar helpCompress(uchar a){
    double aa = a; //uchar似乎在数值运算上有点问题？
    if(aa<64){
        aa = 32;
    }
    else if(aa<128){
        aa = 96;
    }
    else if(aa<192){
        aa = 160;
    }
    else{
        aa = 224;
    }
    return uchar(aa);
}

cv::Mat Compress(cv::Mat img){
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC3);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            out.at<cv::Vec3b>(i,j)[0] = helpCompress(img.at<cv::Vec3b>(i,j)[0]);
            out.at<cv::Vec3b>(i,j)[1] = helpCompress(img.at<cv::Vec3b>(i,j)[1]);
            out.at<cv::Vec3b>(i,j)[2] = helpCompress(img.at<cv::Vec3b>(i,j)[2]);
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Compress(img);
    cv::imshow("t6", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
