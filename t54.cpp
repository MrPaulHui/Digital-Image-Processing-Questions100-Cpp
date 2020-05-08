//模式匹配——Sum of Squared Difference mse
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Point Pattern_Match_SSD(cv::Mat img, cv::Mat T, bool disp=true){
    int height = img.rows;
    int width = img.cols;
    int Th = T.rows;
    int Tw = T.cols;
    int channel = img.channels();
    double min = 1E9;
    double sum;
    cv::Point best_point;
    for(int y=0;y<height-Th;y++){
        for(int x=0;x<width-Tw;x++){
            sum = 0;
            for(int dy=0;dy<Th;dy++){
                for(int dx=0;dx<Tw;dx++){
                    for(int c=0;c<channel;c++){
                        sum += pow((img.at<cv::Vec3b>(y+dy,x+dx)[c]-T.at<cv::Vec3b>(dy,dx)[c]),2); //MSE
                    }
                }
            }
            if(sum<min){
                min = sum;
                best_point.x = x;
                best_point.y = y;
            }
        }
    }
    if(disp){
        cv::Point best_point_counterpart(best_point.x+Tw, best_point.y+Th);
        cv::rectangle(img, best_point, best_point_counterpart, cv::Scalar(0,0,255));
        cv::imshow("match_result", img);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    return best_point;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat T = cv::imread("../imgs/imori_part.jpg", cv::IMREAD_COLOR);
    cv::Point best_p = Pattern_Match_SSD(img, T);
    cout<<"best_match_point_(left top) ("<<best_p.x<<","<<best_p.y<<")"<<endl;
}
