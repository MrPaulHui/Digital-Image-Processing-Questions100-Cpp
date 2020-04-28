//average pooling
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;

cv::Mat AvgPool(cv::Mat img, int pool_size){
    int height = img.rows;
    int width = img.cols;
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i+=pool_size){
        for(int j=0;j<height;j+=pool_size){
            double sum0 = 0;
            double sum1 = 0;
            double sum2 = 0;
            for(int ii=i;ii<i+pool_size;ii++){
                for(int jj=j;jj<j+pool_size;jj++){
                    sum0 += (double)img.at<cv::Vec3b>(ii,jj)[0];
                    sum1 += (double)img.at<cv::Vec3b>(ii,jj)[1];
                    sum2 += (double)img.at<cv::Vec3b>(ii,jj)[2];
                }
            }
            sum0 /= (pool_size*pool_size);
            sum1 /= (pool_size*pool_size);
            sum2 /= (pool_size*pool_size);
            for(int ii=i;ii<i+pool_size;ii++){
                for(int jj=j;jj<j+pool_size;jj++){
                    out.at<cv::Vec3b>(ii,jj)[0] = (uchar)sum0;
                    out.at<cv::Vec3b>(ii,jj)[1] = (uchar)sum1;
                    out.at<cv::Vec3b>(ii,jj)[2] = (uchar)sum2;
                }
            }
        }
    }
    return out;
}

cv::Mat AvgPool_better(cv::Mat img, int pool_size){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i+=pool_size){
        for(int j=0;j<height;j+=pool_size){
            for(int c=0;c<channel;c++){
                double sum = 0;
                for(int ii=i;ii<i+pool_size;ii++){
                    for(int jj=j;jj<j+pool_size;jj++){
                        sum += (double)img.at<cv::Vec3b>(ii,jj)[c];
                    }
                }
                sum /= (pool_size*pool_size);
                for(int ii=i;ii<i+pool_size;ii++){
                    for(int jj=j;jj<j+pool_size;jj++){
                        out.at<cv::Vec3b>(ii,jj)[c] = (uchar)sum;
                    }
                }
            }
        }
    }
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = AvgPool_better(img, 8);
    cv::imshow("t7", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
