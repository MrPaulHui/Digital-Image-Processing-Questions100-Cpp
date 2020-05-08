//模式匹配——Zero-mean Normalization Cross Correlation
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Point Pattern_Match_ZNCC(cv::Mat img, cv::Mat T, bool disp=true){
    int height = img.rows;
    int width = img.cols;
    int Th = T.rows;
    int Tw = T.cols;
    int channel = img.channels();
    double max = -1;
    double sum;
    double region_squared_sum, region_mean;
    double img_mean = 0;
    double T_squared_sum = 0;
    double T_mean = 0;
    for(int dy=0;dy<Th;dy++){
        for(int dx=0;dx<Tw;dx++){
            for(int c=0;c<channel;c++){
                T_squared_sum += pow(T.at<cv::Vec3b>(dy,dx)[c], 2);
                T_mean += T.at<cv::Vec3b>(dy,dx)[c];
            }
        }
    }
    T_mean /= Th*Tw*channel;
    T_squared_sum = sqrt(T_squared_sum+(1-2*Th*Tw*channel)*T_mean*T_mean);
    
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            for(int c=0;c<channel;c++){
                img_mean += img.at<cv::Vec3b>(y,x)[c];
            }
        }
    }
    img_mean /= height*width*channel;
    
    cv::Point best_point;
    for(int y=0;y<height-Th;y++){
        for(int x=0;x<width-Tw;x++){
            sum = 0;
            region_squared_sum = 0;
            /*
            for(int dy=0;dy<Th;dy++){
                for(int dx=0;dx<Tw;dx++){
                    for(int c=0;c<channel;c++){
                        region_squared_sum += pow(img.at<cv::Vec3b>(y+dy,x+dx)[c], 2);
                        region_mean += img.at<cv::Vec3b>(y+dy,x+dx)[c];
                    }
                }
            }
            region_mean /= Th*Tw*channel;
            region_squared_sum = sqrt(region_squared_sum+(1-2*Th*Tw*channel)*region_mean*region_mean);
            for(int dy=0;dy<Th;dy++){
                for(int dx=0;dx<Tw;dx++){
                    for(int c=0;c<channel;c++){
                        sum += (img.at<cv::Vec3b>(y+dy,x+dx)[c]-region_mean)*(T.at<cv::Vec3b>(dy,dx)[c]-T_mean);
                    }
                }
            }
            */
           for(int dy=0;dy<Th;dy++){
                for(int dx=0;dx<Tw;dx++){
                    for(int c=0;c<channel;c++){
                        sum += (img.at<cv::Vec3b>(y+dy,x+dx)[c]-img_mean)*(T.at<cv::Vec3b>(dy,dx)[c]-T_mean);
                        region_squared_sum += pow((img.at<cv::Vec3b>(y+dy,x+dx)[c]-img_mean), 2);
                    }
                }
            }
            region_squared_sum = sqrt(region_squared_sum);
            sum /= (region_squared_sum*T_squared_sum);
            if(sum>max){
                max = sum;
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
    cv::Point best_p = Pattern_Match_ZNCC(img, T);
    cout<<"best_match_point_(left top) ("<<best_p.x<<","<<best_p.y<<")"<<endl;
}
