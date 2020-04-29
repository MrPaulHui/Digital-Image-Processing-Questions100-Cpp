// 高斯滤波
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#include <typeinfo>
using namespace std;

cv::Mat ZeroPadding(cv::Mat img, int pad_size=1){ //C++也可以直接在函数形参列表里给初始值哦
    int h = img.rows;
    int w = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(h+2*pad_size, w+2*pad_size, CV_8UC3);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            for(int c=0;c<channel;c++){
                out.at<cv::Vec3b>(i+pad_size,j+pad_size)[c] = img.at<cv::Vec3b>(i,j)[c];
            }
        }
    }
    return out;
}

cv::Mat Conv(cv::Mat img, cv::Mat kernel, bool need_pad=true){
    // stride只能为1
    int k_size = kernel.rows;
    if(need_pad){
        img = ZeroPadding(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    cv::Mat out = cv::Mat::zeros(height-k_size+1, width-k_size+1, CV_8UC3);
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            for(int c=0;c<channel;c++){
                double sum = 0;
                for(int ii=0;ii<k_size;ii++){
                    for(int jj=0;jj<k_size;jj++){
                        sum += kernel.at<double>(ii,jj) * (double)img.at<cv::Vec3b>(i+ii,j+jj)[c];
                    }
                }
                out.at<cv::Vec3b>(i,j)[c] = (uchar)sum;
            }
        }
    }
    return out;
}

cv::Mat get_guassian_kernel(double sigma, int k_size){
    double kernel[k_size][k_size];
    double pad = floor(k_size/2);
    int _x = 0, _y = 0;
    double kernel_sum = 0;
    for (int y = 0; y < k_size; y++){
        for (int x = 0; x < k_size; x++){
            _y = y - pad;
            _x = x - pad; 
            kernel[y][x] = 1 / (2 * M_PI * sigma * sigma) * exp( - (_x * _x + _y * _y) / (2 * sigma * sigma));
            kernel_sum += kernel[y][x];
        }
    }
    for (int y = 0; y < k_size; y++){
        for (int x = 0; x < k_size; x++){
            kernel[y][x] /= kernel_sum; //归一化
        }
    }
    cv::Mat kernel_mat(k_size, k_size, CV_64FC1, kernel); //直接返回kernel_mat会出问题，用下面两种方法可以解决
    /*
    cv::Mat kernel_mat = cv::Mat::zeros(k_size, k_size, CV_64FC1);
    for (int y = 0; y < k_size; y++){
        for (int x = 0; x < k_size; x++){
            kernel_mat.at<double>(y,x) = kernel[y][x];
        }
    }
    return kernel_mat;
    */
    cv::Mat out = kernel_mat.clone();
    return out;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_noise.jpg", cv::IMREAD_COLOR);
    double kernelo[3][3] = {
        {1.0/16.0, 2.0/16.0, 1.0/16.0},
        {2.0/16.0, 4.0/16.0, 2.0/16.0},
        {1.0/16.0, 2.0/16.0, 1.0/16.0}, //注意这里必须加.0，因为c++里整数相除默认为整除
    };
    cout<<typeid(kernelo).name()<<endl;
    cv::Mat kernel(3,3,CV_64FC1,kernelo); //用具体值初始化Mat的方法，64F对应double，32F对应float
    cout<<kernel<<endl;
    cv::Mat kernel2 = get_guassian_kernel(1.3, 3);
    cout<<kernel2<<endl;
    cv::Mat out = Conv(img,kernel2);
    cv::imshow("t9", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
