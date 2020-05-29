//霍夫变换——检测圆
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
    int a,b,r;
};

inline bool cmp(const r_t_items& a, const r_t_items& b){
    return a.val > b.val;
}


void Hough_Transform_circle(cv::Mat img, int Rmin=0, int Rmax=0, int topN=10, bool disp_middle=true){
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
    int rmin, rmax;
    rmin = Rmin;
    if(Rmax==0){
        rmax = sqrt(height*height+width*width);//对角线长度，是可能最长的r
    }
    else{
        rmax = Rmax;
    }
    int angle_range = 360;
    int a, b;
    double rad;
    vector<int> r_table(rmax-rmin);
    vector<vector<int> > b_table(height+2*rmax, r_table);
    vector<vector<vector<int> > > hough_table(width+2*rmax,b_table);
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(edge.at<uchar>(i,j) != 255) continue; //只对边缘点进行操作，这也是先进行一步边缘检测的目的
            for(int angle=0;angle<angle_range;angle++){
                rad = angle/180.0 * M_PI;
                for(int r=rmin;r<rmax;r++){
                    a = int(j-r*cos(rad));
                    b = int(i-r*sin(rad));
                    hough_table[a+rmax][b+rmax][r-rmin] += 1;
                }
            }
        }
    }

    //step 3. nms and get topN circles
    bool flag;
    vector<r_t_items> circles;
    for(int i=0;i<width+2*rmax;i++){
        for(int j=0;j<height+2*rmax;j++){
            for(int k=0;k<rmax-rmin;k++){
                flag = true;
                for(int dy=-1;dy<2;dy++){
                    for(int dx=-1;dx<2;dx++){
                        for(int dz=-1;dz<2;dz++){
                            if(dy==0 && dx==0 && dz==0) continue;
                            if(val_in(i+dy,0,width+2*rmax-1)==false || val_in(j+dx,0,height+2*rmax-1)==false || val_in(k+dz,0,rmax-rmin-1)) continue;
                            if(hough_table[i][j][k]<hough_table[i+dy][j+dx][k+dz]){
                                flag = false;
                                break;
                            }
                        }
                        if(flag==false)break;
                    }
                    if(flag==false)break;
                }
                if(flag){
                    r_t_items line;
                    line.val = hough_table[i][j][k];
                    line.a = i;
                    line.b = j;
                    line.r = k;
                    circles.push_back(line);
                }
            }
        }
    }
    sort(circles.begin(), circles.end(), cmp);

    //这一部分C++的知识，参考：https://blog.csdn.net/qq_28584889/article/details/88379175
    //step 4. draw circles
    r_t_items circle_item;
    double aa,bb,rr;
    double _cos, _sin;
    int y1, y2;
    for(int i=0;i<topN;i++){
        circle_item = circles[i];
        rr = circle_item.r+rmin;
        aa = circle_item.a-rmax;
        bb = circle_item.b-rmax;
        for(int x=0;x<width;x++){
            y1 = bb + sqrt(rr*rr-(x-aa)*(x-aa));
            y2 = bb - sqrt(rr*rr-(x-aa)*(x-aa));
            if(val_in(y1, 0, height-1)){
                img.at<cv::Vec3b>(y1,x) = cv::Vec3b(0, 0, 255);
            }
            if(val_in(y2, 0, height-1)){
                img.at<cv::Vec3b>(y2,x) = cv::Vec3b(0, 0, 255);
            }
        }
    }
    cv::imshow("hough_circle", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main(){
    cv::Mat img = cv::imread("../imgs/football.jpg", cv::IMREAD_COLOR);
    Hough_Transform_circle(img, 200, 300, 10);
}

