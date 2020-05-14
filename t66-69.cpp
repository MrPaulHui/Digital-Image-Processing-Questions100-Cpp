//HOG 方向梯度直方图
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;

cv::Mat HOG(cv::Mat img, int N=8, int C=3, double eps=1, bool vis=true, bool disp_middle=true){
    int height = img.rows;
    int width = img.cols;
    cv::Mat gray = ToGray(img);
    //step 1. compute grad magnitude and angle
    double grad_kernel_x[3][3] = {
        {0,0,0},
        {-1,0,1},
        {0,0,0}
    };
    double grad_kernel_y[3][3] = {
        {0,-1,0},
        {0,0,0},
        {0,1,0}
    };
    cv::Mat kernel_col(3,3,CV_64FC1,grad_kernel_x);
    cv::Mat kernel_row(3,3,CV_64FC1,grad_kernel_y);
    cv::Mat grad_col = Conv_Gray_HD(gray, kernel_col);
    cv::Mat grad_row = Conv_Gray_HD(gray, kernel_row);
    cv::Mat grad_magnitude = cv::Mat::zeros(height, width, CV_64FC1); 
    cv::Mat grad_direction = cv::Mat::zeros(height, width, CV_8UC1);
    double grad_y, grad_x, grad_m, grad_d, angle;
    int angle_index;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            grad_y = grad_row.at<double>(i,j);
            grad_x = grad_col.at<double>(i,j);
            //step 1.1. get magnitude
            grad_m = sqrt(grad_y*grad_y+grad_x*grad_x);
            grad_magnitude.at<double>(i,j) = grad_m;
            //step 1.2. get and quantize direction angle
            grad_d = atan2(grad_y, grad_x); //返回值是弧度值
            angle = grad_d / M_PI * 180;
            if(angle<0){
                angle += 180;
            }
            angle_index = (int)angle/20;
            grad_direction.at<uchar>(i,j) = angle_index;
        }
    }
    if(disp_middle){
        double minval, maxval;
        cv::minMaxIdx(grad_magnitude, &minval, &maxval);
        cv::Mat disp_grad_magnitude = cv::Mat::zeros(height,width,CV_8UC1);
        cv::Vec3b colors[9] = {cv::Vec3b(255,0,0),cv::Vec3b(0,255,0),cv::Vec3b(0,0,255),
                               cv::Vec3b(255,255,0),cv::Vec3b(255,0,255),cv::Vec3b(0,255,255),
                               cv::Vec3b(127,127,0),cv::Vec3b(127,0,127),cv::Vec3b(0,127,127)};
        cv::Mat disp_grad_direction = cv::Mat::zeros(height,width,CV_8UC3);
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                disp_grad_magnitude.at<uchar>(i,j) = grad_magnitude.at<double>(i,j)/maxval*255;
                disp_grad_direction.at<cv::Vec3b>(i,j) = colors[grad_direction.at<uchar>(i,j)];
            }
        }
        cv::imshow("disp_grad_magnitude", disp_grad_magnitude);
        cv::waitKey(0);
        cv::destroyAllWindows();
        cv::imshow("disp_grad_direction", disp_grad_direction);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 2. cell hist of gradient
    int cell_height = height/N;
    int cell_width = width/N;
    //cv::Mat hist_cell = cv::Mat::zeros(cell_height, cell_width, CV_64FC9);
    double hist_cell[cell_height][cell_width][9] = {0};
    for(int i=0;i<cell_height;i++){
        for(int j=0;j<cell_width;j++){
            for(int ii=0;ii<N;ii++){
                for(int jj=0;jj<N;jj++){
                    //hist_cell.at<cv::Vec9d>(i,j)[grad_direction.at<uchar>(i*N+ii,j*N+jj)] += grad_magnitude.at<double>(i*N+ii,j*N+jj);
                    hist_cell[i][j][grad_direction.at<uchar>(i*N+ii,j*N+jj)] += grad_magnitude.at<double>(i*N+ii,j*N+jj);
                }
            }
        }
    }
    if(vis){
        //有待进一步改进，但是不想搞了
        cv::Mat img_hog_disp = img.clone();
        double hist_cell_normed[cell_height][cell_width][9];
        double cell_norm;
        int cy, cx, y1, x1, y2, x2;
        double rad;
        int brightness;
        for(int y=0;y<cell_height;y++){
            for(int x=0;x<cell_width;x++){
                cell_norm = 0;
                cy = y*N + N/2;
                cx = x*N + N/2;
                x1 = cx - N/4 + 1;
                x2 = cx + N/4 - 1;
                for(int c=0;c<9;c++){
                    cell_norm += hist_cell[y][x][c] * hist_cell[y][x][c];
                }
                for(int c=0;c<9;c++){
                    hist_cell_normed[y][x][c] = hist_cell[y][x][c] / sqrt(cell_norm+eps);
                    rad = (c*20);
                    if(rad>90){
                        rad -= 180;
                    }
                    rad = rad/180*M_PI;
                    y1 = cy - (cx-x1)*tan(rad);
                    y2 = cy + (x2-cx)*tan(rad);
                    brightness = 255 * hist_cell_normed[y][x][c];
                    cv::line(img_hog_disp, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar(brightness,brightness,brightness));
                }
            }
        }
        cv::imshow("img_hog_disp", img_hog_disp);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    //step 3. normalized block hist
    //通道之间也做归一化？ 是的，通道归一化和区域归一化一块进行
    int block_height = cell_height-C+1;
    int block_width = cell_width-C+1;
    double hist_block_normed[block_height][block_width][9*C*C];
    double norm;
    for(int i=0;i<block_height;i++){
        for(int j=0;j<block_width;j++){
            norm = 0;
            for(int dy=0;dy<C;dy++){
                for(int dx=0;dx<C;dx++){
                    for(int c=0;c<9;c++){
                        norm += hist_cell[i+dy][j+dx][c] * hist_cell[i+dy][j+dx][c];
                    }
                }
            }
            norm = sqrt(norm+eps);
            for(int dy=0;dy<C;dy++){
                for(int dx=0;dx<C;dx++){
                    for(int c=0;c<9;c++){
                        hist_block_normed[i][j][(dy*C+dx)*9+c] = hist_cell[i+dy][j+dx][c]/norm;
                    }
                }
            }
        }
    }
    //output hog descriptor
    cv::Mat hog_descriptor = cv::Mat::zeros(block_height*block_width*9*C*C, 1, CV_64FC1);
    for(int i=0;i<block_height;i++){
        for(int j=0;j<block_width;j++){
            for(int c=0;c<9*C*C;c++){
                hog_descriptor.at<double>((i*block_height+j)*9*C*C+c, 1) = hist_block_normed[i][j][c];
            }
        }
    }
    return hog_descriptor;
}

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat descriptor = HOG(img);
    cout<<descriptor<<endl;
}
