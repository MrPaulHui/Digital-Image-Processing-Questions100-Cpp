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

cv::Mat Conv(cv::Mat img, cv::Mat kernel, bool need_pad=true, bool need_clip=true){
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

cv::Mat Conv_Gray_HD(cv::Mat img, cv::Mat kernel, bool need_pad=true){
    // stride只能为1
    // 输出高精度的Mat结果，用于需要高精度的梯度计算，如sobel等。
    int k_size = kernel.rows;
    if(need_pad){
        img = ZeroPadding_Gray(img, floor(k_size/2));
    }
    int height = img.rows;
    int width = img.cols;
    cv::Mat out = cv::Mat::zeros(height-k_size+1, width-k_size+1, CV_64FC1);
    for(int i=0;i<height-k_size+1;i++){
        for(int j=0;j<width-k_size+1;j++){
            double sum = 0;
            for(int ii=0;ii<k_size;ii++){
                for(int jj=0;jj<k_size;jj++){
                    sum += kernel.at<double>(ii,jj) * (double)img.at<uchar>(i+ii,j+jj);
                }
            }
            out.at<double>(i,j) = sum;
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
    cv::Mat kernel_mat(k_size, k_size, CV_64FC1, kernel); //直接返回kernel_mat会出问题，用下面方法可以解决
    cv::Mat out = kernel_mat.clone();
    return out;
}

cv::Mat Guassian_Filter(cv::Mat img, double sigma, int k_size){
    cv::Mat kernel = get_guassian_kernel(sigma, k_size);
    int channel = img.channels();
    if(channel==1){
        return Conv_Gray(img, kernel,true,true);
    }
    else{
        return Conv(img, kernel,true,true);
    }
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

inline int clip_val(int x, int min, int max){ //inline作用：https://www.runoob.com/w3cnote/cpp-inline-usage.html
    if(x<min){
        x=min;
    }
    if(x>max){
        x=max;
    }
    return x;
}

inline bool val_in(int x, int min, int max){
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
            //ori_i = (-c*j + a*i)/det - ty;
            //ori_j = (d*j - b*i)/det - tx;
            ori_i = (-c*j + a*i + c*tx - a*ty)/det; //官方教程给的是不对的，应该先求出来整体的仿射变换逆矩阵，教程给的方法对变换顺序敏感
            ori_j = (d*j - b*i + b*ty - d*tx)/det;
            if(val_in(ori_i, 0, height-1) && val_in(ori_j, 0, width-1)){
                for(int c=0;c<channel;c++){
                    out.at<cv::Vec3b>(i, j)[c] = img.at<cv::Vec3b>(ori_i, ori_j)[c];
                }
            }
        }
    }
    return out;
}

cv::Mat Canny(cv::Mat img, double high_thresh=100, double low_thresh=20, bool disp_middle=false){
    int height = img.rows;
    int width = img.cols;
    //step 1. to gray
    cv::Mat grays = ToGray(img);
    //step 2. guassian filter
    cv::Mat gray = Guassian_Filter(grays, 1.4, 5);
    //step 3. sobel filter to get gradient fx and fy
    double sobel_filter_row[3][3] = {
        {-1.0, -2.0, -1.0},
        {0, 0, 0},
        {1.0, 2.0, 1.0},
    };
    cv::Mat sobel_kernel_row(3,3,CV_64FC1,sobel_filter_row);
    double sobel_filter_col[3][3] = {
        {-1.0, 0, 1.0},
        {-2.0, 0, 2.0},
        {-1.0, 0, 1.0},
    };
    cv::Mat sobel_kernel_col(3,3,CV_64FC1,sobel_filter_col);
    cv::Mat grad_row = Conv_Gray_HD(gray, sobel_kernel_row); //算梯度应定要用高精度卷积，要不然很影响下面幅度和角度的计算
    cv::Mat grad_col = Conv_Gray_HD(gray, sobel_kernel_col);
    //step 4. get grad magnitude and direction
    //cv::Mat grad_magnitude = Clip(out_row + out_col); //这一步是近似运算
    cv::Mat grad_magnitude = cv::Mat::zeros(height, width, CV_8UC1); 
    //这一步数据类型存疑啊，uchar是不是精度太低了，保存的可是梯度啊，包括conv函数里的似乎也有这个问题.
    //ps:解决了哈哈，确实有这个问题，conv加大精度之后解决了,不过这里就用8精度
    cv::Mat grad_direction = cv::Mat::zeros(height, width, CV_8UC1);
    double grad_y, grad_x, grad_m, grad_d, angle;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            grad_y = grad_row.at<double>(i,j);
            grad_x = grad_col.at<double>(i,j);
            //step 4.1. get magnitude
            grad_m = sqrt(grad_y*grad_y+grad_x*grad_x);
            grad_magnitude.at<uchar>(i,j) = (uchar)clip_val(grad_m, 0, 255);
            //step 4.2. get and quantizaiton direction angle
            grad_d = atan2(grad_y, grad_x); //返回值是弧度值
            angle = grad_d / M_PI * 180;
            if(angle < -22.5){
                angle = 180 + angle;
            }
            else if(angle >= 157.5){
                angle = angle - 180;
            } //将角度值范围控制在-22.5_157.5
            //quantizaiton
            if (angle <= 22.5){
                grad_direction.at<uchar>(i,j) = 0;
            } else if (angle <= 67.5){
                grad_direction.at<uchar>(i,j) = 45;
            } else if (angle <= 112.5){
                grad_direction.at<uchar>(i,j) = 90;
            } else {
                grad_direction.at<uchar>(i,j) = 135;
            }
        }
    }
    if(disp_middle){
        cv::imshow("grad_magnitude", grad_magnitude);
        cv::waitKey(0);
        cv::destroyAllWindows();
        cv::imshow("grad_direction", grad_direction);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 5. nms to make edge thin
    cv::Mat edge_thin = cv::Mat::zeros(height, width, CV_8UC1);
    double direction, magnit;
    int dx1, dy1, dx2, dy2;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            magnit = grad_magnitude.at<uchar>(i,j);
            direction = grad_direction.at<uchar>(i,j);
            if(direction==0){
                dx1 = -1;
                dy1 = 0;
                dx2 = 1;
                dy2 = 0;
            }
            if(direction==45){
                dx1 = -1;
                dy1 = -1;
                dx2 = 1;
                dy2 = 1;
            }
            if(direction==90){
                dx1 = 0;
                dy1 = -1;
                dx2 = 0;
                dy2 = 1;
            }
            if(direction==135){
                dx1 = -1;
                dy1 = 1;
                dx2 = 1;
                dy2 = -1;
            }
            //考虑边界情况
            if (j == 0){
                dx1 = fmax(dx1, 0);
                dx2 = fmax(dx2, 0);
            }
            if (j == (width - 1)){
                dx1 = fmin(dx1, 0);
                dx2 = fmin(dx2, 0);
            }
            if (i == 0){
                dy1 = fmax(dy1, 0);
                dy2 = fmax(dy2, 0);
            }
            if (i == (height - 1)){
                dy1 = fmin(dy1, 0);
                dy2 = fmin(dy2, 0);
            }
            if(magnit >= grad_magnitude.at<uchar>(i+dy1,j+dx1) && magnit >= grad_magnitude.at<uchar>(i+dy2,j+dx2)){
                edge_thin.at<uchar>(i,j) = (uchar)magnit;
            }
        }
    }
    if(disp_middle){
        cv::imshow("edge_thin", edge_thin);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 6. using two thresholds to binarize
    cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);
    double val;
    bool flag;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            val = (double)edge_thin.at<uchar>(i,j);
            if(val>=high_thresh){
                out.at<uchar>(i,j) = 255;
            }
            else if(val<=low_thresh){
                continue;
            }
            else{
                flag = false;
                for(int dyy=-1;dyy<2;dyy++){
                    for(int dxx=-1;dxx<2;dxx++){
                        if(dyy==0 && dxx==0) continue;
                        if(val_in(i+dyy,0,height-1)==false || val_in(j+dxx,0,width-1)==false) continue;
                        if((double)edge_thin.at<uchar>(i+dyy,j+dxx)>=high_thresh){
                            flag = true;
                            break;
                        }
                    }
                    if(flag) break;
                }
                if(flag){
                    out.at<uchar>(i,j) = 255;
                }
            }
        }
    }
    return out;
}

cv::Mat Ostu_Threshold_two(cv::Mat img){
    cv::Mat gray = ToGray(img);
    int height = gray.rows;
    int width = gray.cols;
    double w0=0, w1=0, u0=0, u1=0, max_g=0;
    int th;
    for(int t=0; t<255; t++){
        w0=0;
        w1=0;
        u0=0;
        u1=0;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                int value = int(gray.at<uchar>(i, j));
                if(value<t){
                    w0++;
                    u0 += value;
                }
                else{
                    w1++;
                    u1 += value;
                }
            }
        }
        u0 /= w0;
        u1 /= w1;
        w0 /= (height*width);
        w1 /= (height*width);
        double g = w0 * w1 * pow((u0-u1), 2);
        if(g>max_g){
            max_g=g;
            th = t;
        }
    }    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(gray.at<uchar>(i,j)<=th){
                gray.at<uchar>(i,j) = 0;
            }
            else{
                gray.at<uchar>(i,j) = 255;
            }
        }
    }
    return gray;
}

cv::Mat dilate(cv::Mat binary_, int N=1){
    cv::Mat binary = binary_.clone();
    int height = binary.rows;
    int width = binary.cols;
    for(int n=0;n<N;n++){
        cv::Mat tmp = binary.clone();
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(tmp.at<uchar>(i,j)==255)continue;
                if(j-1>=0 && tmp.at<uchar>(i,j-1)==255){
                    binary.at<uchar>(i,j) = 255;
                    continue;
                }
                if(j+1<width && tmp.at<uchar>(i,j+1)==255){
                    binary.at<uchar>(i,j) = 255;
                    continue;
                }
                if(i-1>=0 && tmp.at<uchar>(i-1,j)==255){
                    binary.at<uchar>(i,j) = 255;
                    continue;
                }
                if(i+1<height && tmp.at<uchar>(i+1,j)==255){
                    binary.at<uchar>(i,j) = 255;
                    continue;
                }
            }
        }
    }
    return binary;
}

cv::Mat erode(cv::Mat binary_, int N=1){
    cv::Mat binary = binary_.clone();
    int height = binary.rows;
    int width = binary.cols;
    for(int n=0;n<N;n++){
        cv::Mat tmp = binary.clone();
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                if(tmp.at<uchar>(i,j)==0)continue;
                if(j-1>=0 && tmp.at<uchar>(i,j-1)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
                if(j+1<width && tmp.at<uchar>(i,j+1)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
                if(i-1>=0 && tmp.at<uchar>(i-1,j)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
                if(i+1<height && tmp.at<uchar>(i+1,j)==0){
                    binary.at<uchar>(i,j) = 0;
                    continue;
                }
            }
        }
    }
    return binary;
}

cv::Mat opening_closing_op(cv::Mat binary, int N=1, bool open=true){
    if(open)return dilate(erode(binary, N), N);
    else return erode(dilate(binary, N), N);
}

cv::Mat bilinear_interpolation(cv::Mat img, double scale_y, double scale_x){
    int height = img.rows;
    int width = img.cols;
    int channel = img.channels();
    int out_height = (int)(height*scale_y);
    int out_width = (int)(width*scale_x);
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC3);
    int x, y;
    double dx, dy;
    double val;
    for(int i=0;i<out_height;i++){
        y = floor(i/scale_y);
        dy = i/scale_y - y;
        for(int j=0;j<out_width;j++){
            x = floor(j/scale_x);
            dx = j/scale_x - x;
            for(int c=0;c<channel;c++){
                val = (1-dx)*(1-dy)*(double)img.at<cv::Vec3b>(y,x)[c] + dx*(1-dy)*(double)img.at<cv::Vec3b>(y,x+1)[c] + 
                      (1-dx)*dy*(double)img.at<cv::Vec3b>(y+1,x)[c] + dx*dy*(double)img.at<cv::Vec3b>(y+1,x+1)[c];
                out.at<cv::Vec3b>(i,j)[c] = (uchar)val;
            }
        }
    }
    return out;
}

cv::Mat bilinear_interpolation_gray(cv::Mat img, double scale_y, double scale_x){
    int height = img.rows;
    int width = img.cols;
    int out_height = (int)(height*scale_y);
    int out_width = (int)(width*scale_x);
    cv::Mat out = cv::Mat::zeros(out_height, out_width, CV_8UC1);
    int x, y;
    double dx, dy;
    double val;
    for(int i=0;i<out_height;i++){
        y = floor(i/scale_y);
        dy = i/scale_y - y;
        for(int j=0;j<out_width;j++){
            x = floor(j/scale_x);
            dx = j/scale_x - x;
            val = (1-dx)*(1-dy)*(double)img.at<uchar>(y,x) + dx*(1-dy)*(double)img.at<uchar>(y,x+1) + 
                      (1-dx)*dy*(double)img.at<uchar>(y+1,x) + dx*dy*(double)img.at<uchar>(y+1,x+1);
            out.at<uchar>(i,j) = (uchar)val;
        }
    }
    return out;
}

inline int connected_number_pixel(cv::Mat img, int i, int j, bool four=true){
    int height = img.rows;
    int width = img.cols;
    int x1,x2,x3,x4,x5,x6,x7,x8;
    int S;
    x1 = img.at<uchar>(i,fmin(j+1,width-1))/255; //要时刻注意边界情况
    x2 = img.at<uchar>(fmax(i-1,0),fmin(j+1,width-1))/255;
    x3 = img.at<uchar>(fmax(i-1,0),j)/255;
    x4 = img.at<uchar>(fmax(i-1,0),fmax(j-1,0))/255;
    x5 = img.at<uchar>(i,fmax(j-1,0))/255;
    x6 = img.at<uchar>(fmin(i+1,height-1),fmax(j-1,0))/255;
    x7 = img.at<uchar>(fmin(i+1,height-1),j)/255;
    x8 = img.at<uchar>(fmin(i+1,height-1),fmin(j+1,width-1))/255;
    if(four==false){
        x1 = 1-x1;
        x2 = 1-x2;
        x3 = 1-x3;
        x4 = 1-x4;
        x5 = 1-x5;
        x6 = 1-x6;
        x7 = 1-x7;
        x8 = 1-x8;
    }
    S = (x1-x1*x2*x3)+(x3-x3*x4*x5)+(x5-x5*x6*x7)+(x7-x7*x8*x1);
    return S;
}

cv::Mat RGB2HSV(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    float h, s, v;
    float r, g, b;
    float _max, _min;
    cv::Mat hsv = cv::Mat::zeros(height, width, CV_32FC3); //CV_32FC3表示每个像素值为32bit，F表示float类型，对应U表示unsigned int，C3表示3个通道
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            r = (float)img.at<cv::Vec3b>(i,j)[2]/255;
            g = (float)img.at<cv::Vec3b>(i,j)[1]/255; // 不加float类型转换，会出错，C++的类型一定要注意！！！
            b = (float)img.at<cv::Vec3b>(i,j)[0]/255; // 0:b, 1:g, 2:r
            _max = fmax(r, fmax(g, b));
            _min = fmin(r, fmin(g, b));
            // get h
            if(_max == _min){
                h = 0;
            } else if(_min == b){
                h = 60*(g-r)/(_max-_min)+60;
            } else if(_min == r){
                h = 60*(b-g)/(_max-_min)+180;
            } else if(_min == g){
                h = 60*(r-b)/(_max-_min)+300;
            }
            // get s
            s = _max - _min;
            // get v
            v = _max;
            hsv.at<cv::Vec3f>(i,j)[0] = h;
            hsv.at<cv::Vec3f>(i,j)[1] = s;
            hsv.at<cv::Vec3f>(i,j)[2] = v;
        }
    }
    return hsv;
}

cv::Mat HSV2RGB(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    float h, s, v;
    float r, g, b;
    cv::Mat rgb = cv::Mat::zeros(height, width, CV_8UC3);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            h = img.at<cv::Vec3f>(i,j)[0];
            s = img.at<cv::Vec3f>(i,j)[1];
            v = img.at<cv::Vec3f>(i,j)[2];
            float c = s;
            float h1 = h/60;
            float x = c*(1-abs(fmod(h1,2)-1));
            r = g = b = v - c;
            if(h1<1){
                r += c;
                g += x;
            }
            else if(h1<2){
                r += x;
                g += c;
            }
            else if(h1<3){
                g += c;
                b += x;
            }
            else if(h1<4){
                g += x;
                b += c;
            }
            else if(h1<5){
                r += x;
                b += c;
            }
            else if(h1<6){
                r += c;
                b += x;
            }
            rgb.at<cv::Vec3b>(i,j)[0] = uchar(b*255);
            rgb.at<cv::Vec3b>(i,j)[1] = uchar(g*255);
            rgb.at<cv::Vec3b>(i,j)[2] = uchar(r*255);
        }
    }
    return rgb;
}
