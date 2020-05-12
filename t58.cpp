//连通域——4邻接
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
#include <unordered_map>
using namespace std;
/*傻逼算法，废了我一天的时间，妈的！
cv::Mat connected_component_label_4(cv::Mat img_){
    //bug太多了！！！
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
    //core algo, generate LUT
    int LUT[height*width] = {0}; //look up table
    int now_label = 1;
    int tmp_min;
    bool flag_top, flag_left;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(img.at<double>(i,j)==0) continue;
            if((i-1<0 || img.at<double>(i-1,j)==0) && (j-1<0 || img.at<double>(i,j-1)==0)){
                img.at<double>(i,j) = now_label;
                LUT[now_label] = now_label;
                now_label ++;
            }
            else{
                tmp_min = 100000;
                flag_top = false;
                flag_left = false;
                if(i-1>=0 && img.at<double>(i-1,j)>0){
                    tmp_min = fmin(tmp_min, img.at<double>(i-1,j));
                    flag_top = true;
                }
                if(j-1>=0 && img.at<double>(i,j-1)>0){
                    tmp_min = fmin(tmp_min, img.at<double>(i,j-1));
                    flag_left = true;
                }
                img.at<double>(i,j) = tmp_min;
                if(flag_top){
                    if(LUT[(int)img.at<double>(i-1,j)]==0 || LUT[(int)img.at<double>(i-1,j)]>tmp_min){ //防止已经连通到其他区域的被重新赋值所取代
                        LUT[(int)img.at<double>(i-1,j)] = tmp_min;
                    }
                }
                if(flag_left){
                    if(LUT[(int)img.at<double>(i,j-1)]==0 || LUT[(int)img.at<double>(i,j-1)]>tmp_min){
                        LUT[(int)img.at<double>(i,j-1)] = tmp_min;
                    }
                }
            }
        }
    }
    for(int i=0;i<height*width;i++){
        if(LUT[i]>0){
            cout<<i<<" "<<LUT[i]<<endl;
        }
    }
    cout<<"two"<<endl;
    // merge LUT e.g. LUT[2]=1, LUT[3]=2, so we should set LUT[3]=1
    for(int i=1;i<height*width;i++){
        if(LUT[i]==0) break;
        if(LUT[i]==i){
            continue;
        }
       while(LUT[LUT[i]]!=LUT[i]){
           LUT[i] = LUT[LUT[i]];
       }
    }
    for(int i=0;i<height*width;i++){
        if(LUT[i]>0){
            cout<<i<<" "<<LUT[i]<<endl;
        }
    }
    cout<<"three"<<endl;
    // minimize label e.g. LUT[1]=1, LUT[2]=1, LUT[3]=3, so we should set LUT[3]=2
    int count = 1;
    int ii=1;
    int jj;
    int tmp;
    unordered_map<int,int> hash;
    while(ii<height*width){
        if(LUT[ii]==0) break;
        if(hash.find(LUT[ii])==hash.end()){
            hash[LUT[ii]] = count;
            LUT[ii] = count;
            count ++;
        }
        else{
            LUT[ii] = hash[LUT[ii]];
        }
        ii ++;
    }
    for(int i=0;i<height*width;i++){
        if(LUT[i]>0){
            cout<<i<<" "<<LUT[i]<<endl;
        }
    }
    // generate colors list for disp
    cv::Vec3b colors[count];
    colors[0] = cv::Vec3b(0,0,0);
    for(int i=1;i<count;i++){
        cv::RNG rng(i*10); // opencv random
        colors[i] = cv::Vec3b(rng.uniform(128,255),rng.uniform(128,255),rng.uniform(128,255));
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(LUT[(int)img.at<double>(i,j)]!=4)continue;
            out.at<cv::Vec3b>(i,j) = colors[LUT[(int)img.at<double>(i,j)]];
        }
    }
    return out;
}
*/
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
}

cv::Mat connected_component_label_4_dfs(cv::Mat img_){
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
    cout<<label<<endl;
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
    cv::Mat img = cv::imread("../imgs/digit.png", cv::IMREAD_COLOR);
    cv::Mat out = connected_component_label_4_dfs(img);
    cv::imshow("out", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
