//连通域——8邻接
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
using namespace std;

inline void dfs(cv::Mat &img, int height, int width, int y, int x, int label){
    img.at<double>(y,x) = label;
    if(y-1>=0 && img.at<double>(y-1,x)==255){
        dfs(img, height, width, y-1, x, label);
    }
    if(y+1<height && img.at<double>(y+1,x)==255){
        dfs(img, height, width, y+1, x, label);
    }
    if(x-1>=0 && img.at<double>(y,x-1)==255){
        dfs(img, height, width, y, x-1, label);
    }
    if(x+1<width && img.at<double>(y,x+1)==255){
        dfs(img, height, width, y, x+1, label);
    }
    if(y-1>=0 && x-1>=0 && img.at<double>(y-1,x-1)==255){
        dfs(img, height, width, y-1, x-1, label);
    }
    if(y-1>=0 && x+1<width && img.at<double>(y-1,x+1)==255){
        dfs(img, height, width, y-1, x+1, label);
    }
    if(y+1<height && x-1>=0 && img.at<double>(y+1,x-1)==255){
        dfs(img, height, width, y+1, x-1, label);
    }
    if(y+1<height && x+1<width && img.at<double>(y+1,x+1)==255){
        dfs(img, height, width, y+1, x+1, label);
    }
}

cv::Mat connected_component_label_8_dfs(cv::Mat img_){
    //img为二值化图像，channel为1或3
    int height = img_.rows;
    int width = img_.cols;
    cv::Mat img;
    if(img_.channels()==3){
        Ostu_Threshold_two(img_).convertTo(img, CV_64F);
    }
    else{
        img_.convertTo(img, CV_64F);
    }
    int label = -1;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            if(img.at<double>(y,x)==255){
                dfs(img, height, width, y, x, label);
                label --;
            }
        }
    }
    cout<<-label<<endl;
    // generate colors list for disp
    cv::Vec3b colors[-label];
    colors[0] = cv::Vec3b(0,0,0);
    for(int i=1;i<-label;i++){
        cv::RNG rng(i*10); // opencv random
        colors[i] = cv::Vec3b(rng.uniform(128,255),rng.uniform(128,255),rng.uniform(128,255));
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            out.at<cv::Vec3b>(i,j) = colors[-(int)img.at<double>(i,j)];
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/bin.png", cv::IMREAD_COLOR);
    cv::Mat out = connected_component_label_8_dfs(img);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
