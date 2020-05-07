//霍夫变换——检测直线
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;

//define some data structure to sort for get topN 
struct r_t_items
{
    int val;
    int r, t;
};

inline bool cmp(const r_t_items& a, const r_t_items& b){
    return a.val > b.val;
}


void Hough_Transform_line(cv::Mat img, int topN=10, bool disp_middle=true){
    int height = img.rows;
    int width = img.cols;
    //step 1. edge detection to get edge
    cv::Mat edge = Canny(img, 100, 30);
    if(disp_middle){
        cv::imshow("edge", edge);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 2. generate hough table for vote
    int rmax = sqrt(height*height+width*width);//对角线长度，是可能最长的r
    int angle_range = 180;
    int r;
    double rad;
    int hough_table[rmax*2][angle_range] = {0}; //(r,t)表，统计每个(r,t)出现的个数，即直方图。用来看哪些(r,t)出现的次数最高，则其对应的直线越有可能是图中的直线
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(edge.at<uchar>(i,j) != 255) continue; //只对边缘点进行操作，这也是先进行一步边缘检测的目的
            for(int angle=0;angle<angle_range;angle++){
                rad = angle/180.0 * M_PI;
                r = (int)j*cos(rad)+i*sin(rad);
                hough_table[rmax+r][angle] ++;
            }
        }
    }
    //step 3. nms and get topN lines
    bool flag;
    vector<r_t_items> lines;
    for(int i=0;i<rmax*2;i++){
        for(int j=0;j<angle_range;j++){
            flag = true;
            for(int dy=-1;dy<2;dy++){
                for(int dx=-1;dx<2;dx++){
                    if(dy==0 && dx==0) continue;
                    if(val_in(i+dy,0,rmax*2-1)==false || val_in(j+dx,0,angle_range-1)==false) continue;
                    if(hough_table[i][j]<hough_table[i+dy][j+dx]){
                        flag = false;
                        break;
                    }
                }
                if(flag==false)break;
            }
            if(flag){
                r_t_items line;
                line.val = hough_table[i][j];
                line.r = i;
                line.t = j;
                lines.push_back(line);
            }
        }
    }
    sort(lines.begin(), lines.end(), cmp);
    //这一部分C++的知识，参考：https://blog.csdn.net/qq_28584889/article/details/88379175
    //step 4. draw lines
    r_t_items line_item;
    double rr, tt;
    double _cos, _sin;
    int y, x;
    for(int i=0;i<topN;i++){
        line_item = lines[i];
        rr = line_item.r-rmax;
        tt = line_item.t/180.0*M_PI;
        _cos = cos(tt);
        _sin = sin(tt);
        if(_cos==0){
            cv::line(img, cv::Point(0,(int)rr), cv::Point(width-1,(int)rr), cv::Scalar(0,0,255));
            continue;
        }
        if(_sin==0){
            cv::line(img, cv::Point((int)rr,0), cv::Point((int)rr,height-1), cv::Scalar(0,0,255));
            continue;
        }
        for(int x=0;x<width;x++){
            y = -_cos/_sin*x+rr/_sin;
            if(val_in(y, 0, height-1)){
                img.at<cv::Vec3b>(y,x) = cv::Vec3b(0, 0, 255);
            }
        }
        for(int y=0;y<height;y++){
            x = -_sin/_cos*y+rr/_cos;
            if(val_in(x, 0, width-1)){
                img.at<cv::Vec3b>(y,x) = cv::Vec3b(0, 0, 255);
            }
        }
    }
    cv::imshow("hough_line", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main(){
    cv::Mat img = cv::imread("../imgs/thorino.jpg", cv::IMREAD_COLOR);
    Hough_Transform_line(img, 10);
}
