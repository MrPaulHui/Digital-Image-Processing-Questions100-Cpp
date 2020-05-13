//细化处理
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat thinner(cv::Mat img_){
    int height = img_.rows;
    int width = img_.cols;
    cv::Mat img;
    if(img_.channels()==3){
        img = Ostu_Threshold_two(img_);
    }
    else{
        img = img_.clone();
    }
    int connect_s;
    int four_neigbor_zeros;
    int eight_neighbor_ones;
    bool is_changed = true;
    while(is_changed){
        is_changed = false;
        cv::Mat tmp = img.clone();
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(tmp.at<uchar>(i,j)==0) continue;
                four_neigbor_zeros = 0;
                eight_neighbor_ones = 0;
                for(int dy=-1;dy<2;dy++){
                    for(int dx=-1;dx<2;dx++){
                        if(dy==0 && dx==0) continue;
                        if(val_in(i+dy,0,height-1)==false || val_in(j+dx,0,width-1)==false) continue;
                        //四邻域的0个数
                        if(dy==0 || dx==0){
                            if(tmp.at<uchar>(i+dy,j+dx)==0){
                                four_neigbor_zeros ++;
                            }
                        }
                        //八邻域的1个数
                        if(tmp.at<uchar>(i+dy,j+dx)==255){
                            eight_neighbor_ones ++;
                        }
                    }
                }
                //4-连接数
                connect_s = connected_number_pixel(tmp, i, j, true);
                if(four_neigbor_zeros>=1 && eight_neighbor_ones>3 && connect_s==1){
                    img.at<uchar>(i,j) = 0;
                    is_changed = true;
                }
            }
        }
    }
    return img;
}

int main(){
    cv::Mat img = cv::imread("../imgs/digit.png", cv::IMREAD_COLOR);
    cv::Mat out = thinner(img);
    cv::imshow("t63", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
