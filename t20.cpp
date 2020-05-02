//绘制直方图
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;

int* CalHist(cv::Mat img){
    //三通道的直方图直接统计所有h×w×c个像素
    //int hist[256] = {0}; //只能初始化为0好像，这个好像不太行？
    static int hist[256]; //直接这样定义是不行的，因为没有定义成static的，函数执行完，这个hist就会被释放掉，指针找不到数据
    //参考：https://www.cnblogs.com/Wade-James/p/7965775.html
    for(int i=0;i<256;i++){
        hist[i] = 0;
    }
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int c=0;c<channel;c++){
                hist[(int)img.at<cv::Vec3b>(i,j)[c]] += 1; //试着写成++？
            }
        }
    }
    return hist;
}

void DisplayHist(int* hist, int hist_size=256, int disp_size=512, int x_axis_height=20){
    double max_ = 0;
    for(int i=0;i<hist_size;i++){
        if(*(hist+i) > max_){
            max_ = *(hist+i);
        }
    }
    double hpt = 0.8 * (double)(disp_size-x_axis_height);
    cv::Mat disp(disp_size, disp_size, CV_8UC3, cv::Scalar(255,255,255));
    //cv::line(disp, cv::Point(0,disp_size-x_axis_height), cv::Point(disp_size,disp_size-x_axis_height),cv::Scalar(0,0,0));
    for(int i=0;i<hist_size;i++){
        double val = *(hist+i);
        int heights = (int)(val/max_*hpt);
        cv::line(disp, cv::Point(i,disp_size-x_axis_height), cv::Point(i,disp_size-x_axis_height-heights), cv::Scalar(172, 172, 150)); //需要加上#include "opencv2/imgproc/imgproc.hpp"这个头文件
        if(i%50==0){
            cv::putText(disp, to_string(i), cv::Point(i,disp_size),
                        cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,0));
        }
    }
    cv::imshow("hist", disp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori_dark.jpg", cv::IMREAD_COLOR); //image_dark也是三通道的bgr图
    int* hist = CalHist(img);
    //cout<<*(hist+128)<<endl;
    DisplayHist(hist);
}
