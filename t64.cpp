//细化处理——Hilditch算法
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat thinner_Hilditch(cv::Mat img_){
    int height = img_.rows;
    int width = img_.cols;
    cv::Mat img;
    if(img_.channels()==3){
        Ostu_Threshold_two(img_).convertTo(img, CV_64F);
    }
    else{
        img_.convertTo(img, CV_64F);
    }
    int connect_s;
    int four_neigbor_zeros;
    int eight_neighbor_ones;
    int eight_neighbor_sum;
    bool condition5;
    bool is_changed = true;
    while(is_changed){
        is_changed = false;
        cv::Mat tmp;
        img.convertTo(tmp, CV_8U);
        img = img/255;
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(img.at<double>(i,j)==0) continue;
                //8-连接数
                connect_s = connected_number_pixel(tmp, i, j, false);
                four_neigbor_zeros = 0;
                eight_neighbor_ones = 0;
                eight_neighbor_sum = 0;
                condition5 = true;
                for(int dy=-1;dy<2;dy++){
                    for(int dx=-1;dx<2;dx++){
                        if(dy==0 && dx==0) continue;
                        if(val_in(i+dy,0,height-1)==false || val_in(j+dx,0,width-1)==false) continue;
                        //四邻域的0个数
                        if(dy==0 || dx==0){
                            if(img.at<double>(i+dy,j+dx)==0){
                                four_neigbor_zeros ++;
                            }
                        }
                        //八邻域的1个数
                        if(img.at<double>(i+dy,j+dx)==1){
                            eight_neighbor_ones ++;
                        }
                        //八邻域的绝对值和
                        eight_neighbor_sum += abs(img.at<double>(i+dy,j+dx));
                        //condition5
                        if(condition5==false) continue;
                        if(img.at<double>(i+dy,j+dx)==-1){
                            cv::Mat tmp2 = tmp.clone();
                            tmp2.at<uchar>(i+dy,j+dx) = 0;
                            if(connected_number_pixel(tmp2, i, j, false)!=1){
                                condition5 = false;
                            }
                        }
                    }
                }
                if(four_neigbor_zeros>=1 && eight_neighbor_ones>1 && connect_s==1 && eight_neighbor_sum>2 && condition5){
                    img.at<double>(i,j) = -1;
                    is_changed = true;
                }
            }
        }
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(img.at<double>(i,j)==-1){
                    img.at<double>(i,j) = 0;
                }
                if(img.at<double>(i,j)==1){
                    img.at<double>(i,j) = 255;
                }
            }
        }
    }
    return img;
}

int main(){
    cv::Mat img = cv::imread("../imgs/bin.png", cv::IMREAD_COLOR);
    cv::Mat out = thinner_Hilditch(img);
    cv::imshow("t64", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
