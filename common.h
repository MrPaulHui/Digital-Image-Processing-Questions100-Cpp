#pragma once
#include <math.h>
#include "opencv2/imgproc/imgproc.hpp"
cv::Mat ToGray(cv::Mat img){ //img的格式是bgr
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC1);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            unsigned char b = img.at<cv::Vec3b>(i,j)[0];
            unsigned char g = img.at<cv::Vec3b>(i,j)[1];
            unsigned char r = img.at<cv::Vec3b>(i,j)[2];
            out.at<unsigned char>(i,j) = 0.2126 * (double)r + 0.7152 * (double)g + 0.0722 * (double)b;
        }
    }
    return out;
}

cv::Mat ZeroPadding(cv::Mat img, int pad_size=1){
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

cv::Mat ZeroPadding_Gray(cv::Mat img, int pad_size=1){
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h+2*pad_size, w+2*pad_size, CV_8UC1);
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            out.at<uchar>(i+pad_size,j+pad_size) = img.at<uchar>(i,j);
        }
    }
    return out;
}

cv::Mat Conv_Gray(cv::Mat img, cv::Mat kernel, bool need_pad=true, bool need_clip=true){
    // stride只能为1
    int k_size = kernel.rows;
    if(need_pad){
        img = ZeroPadding_Gray(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    cv::Mat out = cv::Mat::zeros(height-k_size+1, width-k_size+1, CV_8UC1);
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            double sum = 0;
            for(int ii=0;ii<k_size;ii++){
                for(int jj=0;jj<k_size;jj++){
                    sum += kernel.at<double>(ii,jj) * (double)img.at<uchar>(i+ii,j+jj);
                }
            }
            if(need_clip){
                sum = fmax(sum, 0);
                sum = fmin(sum, 255); //需要做一手clip，保证像素值范围在0-255
            }
            out.at<uchar>(i,j) = (uchar)sum;
        }
    }
    return out;
}

cv::Mat Conv(cv::Mat img, cv::Mat kernel, bool need_pad=true, bool need_clip=false){
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
                if(need_clip){
                    sum = fmax(sum, 0);
                    sum = fmin(sum, 255); //需要做一手clip，保证像素值范围在0-255
                }
                out.at<cv::Vec3b>(i,j)[c] = (uchar)sum;
            }
        }
    }
    return out;
}

cv::Mat Clip(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    double v;
    if(channel==3){
        cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                for(int c=0;c<channel;c++){
                    double val = (double)img.at<cv::Vec3b>(i,j)[c];
                    v = fmax(val, 0);
                    v = fmin(val, 255);
                    out.at<cv::Vec3b>(i,j)[c] = (uchar)v;
                }
            }
        }
        return out;
    }
    else{
        cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                double val = (double)img.at<uchar>(i,j);
                v = fmax(val, 0);
                v = fmin(val, 255);
                out.at<uchar>(i,j) = (uchar)v;
            }
        }
        return out;
    }
}

int clip_val(int x, int min, int max){
    if(x<min){
        x=min;
    }
    if(x>max){
        x=max;
    }
    return x;
}

bool val_in(int x, int min, int max){
    if(x>=min && x<=max){
        return true;
    }
    else{
        return false;
    }
}

double* CalHist(cv::Mat img, bool norm=false){
    //三通道的直方图直接统计所有h×w×c个像素
    //int hist[256] = {0}; //只能初始化为0好像，这个好像不太行？
    static double hist[256]; //直接这样定义是不行的，因为没有定义成static的，函数执行完，这个hist就会被释放掉，指针找不到数据
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
    if(norm){
        for(int i=0;i<256;i++){
            hist[i] /= (height*width*channel); //一般来说直方图是需要除总数，即每个表示比例的
        }
    }
    return hist;
}

void DisplayHist(double* hist, int hist_size=256, int disp_size=512, int x_axis_height=20){
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
            cv::putText(disp, std::to_string(i), cv::Point(i,disp_size),
                        cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,0));
        }
    }
    cv::imshow("hist", disp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

double* CalAccHist(cv::Mat img){
    //累积直方图
    double* hist = CalHist(img);
    static double acc_hist[256];
    double accum_hist = 0;
    for(int i=0;i<256;i++){
        accum_hist += *(hist+i);
        acc_hist[i] = accum_hist;
    }
    for(int i=0;i<256;i++){
        acc_hist[i] /= accum_hist; //累积直方图必须是比例值，即要除总数 //如果CalHist做了比例化，这里就不需要了
    }
    return acc_hist;
}

void Hist(cv::Mat img){
    double* hist;
    hist = CalHist(img);
    DisplayHist(hist);
}

cv::Mat hist_equalize(cv::Mat img){
    //参考：https://blog.csdn.net/schwein_van/article/details/84336633
    //其实是灰度级值的变换，比如灰度级为2的像素有100个，这个灰度级均衡化后值变成了4，那么这100个像素点的像素值变换为了4
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    
    double* hist;
    hist = CalHist(img);
    int hist_equalize_val[256]; // 每个灰度级(这里就是每个像素值)对应的均衡化后的像素值
    double coff = 255.0/(height*width*channel);
    double accum_hist = 0;
    for(int i=0;i<256;i++){
        accum_hist += *(hist+i);
        hist_equalize_val[i] = (int)(coff * accum_hist);
    }
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            for(int _c=0;_c<channel;_c++){
                int val = (int)img.at<cv::Vec3b>(i,j)[_c];
                out.at<cv::Vec3b>(i,j)[_c] = (uchar)hist_equalize_val[val];
            }
        }
    }
    return out;
}

cv::Mat Affine(cv::Mat img, double a, double b, double c, double d, double tx, double ty, int want_height=0, int want_width=0){
    //适用于平移和缩放的affine
    //a,b,c,d,tx,ty为仿射变换矩阵的参数
    /*
    变换矩阵：Aff=
    [[a, b, tx],
     [c, d, ty],
     [0, 0, 1]]
    原始坐标 P = (x, y, 1)^T
    变换后坐标 P' = (x', y', 1)^T
    P' = Aff * P
    实现上用反变换，即P = Aff^-1 * P'
    */
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    int ori_i, ori_j;
    int out_height;
    int out_width;
    if(want_height==0){
        out_height = height * d;
    }
    else{
        out_height = want_height;
    }
    if(want_width==0){
        out_width = width * a;
    }
    else{
        out_width = want_width;
    }
    double det = a*d - b*c;
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC3);
    for(int i=0;i<out_height;i++){
        for(int j=0;j<out_width;j++){
            ori_i = (-c*j + a*i)/det - ty;
            ori_j = (d*j - b*i)/det - tx;
            if(val_in(ori_i, 0, height-1) && val_in(ori_j, 0, width-1)){
                for(int c=0;c<channel;c++){
                    out.at<cv::Vec3b>(i, j)[c] = img.at<cv::Vec3b>(ori_i, ori_j)[c];
                }
            }
        }
    }
    return out;
}
