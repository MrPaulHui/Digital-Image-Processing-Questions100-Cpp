//应用仿射变换做人脸对齐
//这一部分的矩阵运算操作直接调用了opencv的库函数，自己实现一遍也可以，只是很繁琐
//关键点使用了cv::Point类表示
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
#include <iostream>
#include <math.h>
using namespace std;

cv::Mat Face_Align_Affine(cv::Mat img, cv::Point* lmks, cv::Point* standard_lmks, int out_size, int lmk_num=5){
    cv::Mat X = cv::Mat::zeros(lmk_num*2, 4, CV_64FC1);
    cv::Mat Y = cv::Mat::zeros(lmk_num*2, 1, CV_64FC1);
    cv::Mat A;
    for(int i=0;i<lmk_num;i++){
        Y.at<double>(i*2,0) = standard_lmks[i].x;
        Y.at<double>(i*2+1,0) = standard_lmks[i].y;
        X.at<double>(i*2,0) = lmks[i].x;
        X.at<double>(i*2,1) = -lmks[i].y;
        X.at<double>(i*2,2) = 1;
        X.at<double>(i*2,3) = 0;
        X.at<double>(i*2+1,0) = lmks[i].y;
        X.at<double>(i*2+1,1) = lmks[i].x;
        X.at<double>(i*2+1,2) = 0;
        X.at<double>(i*2+1,3) = 1;
    }
    A = (X.t()*X).inv()*X.t()*Y;
    double sc, ss, tx, ty;
    sc = A.at<double>(0,0);
    ss = A.at<double>(1,0);
    tx = A.at<double>(2,0);
    ty = A.at<double>(3,0);
    cout<<"align_matrix: a: "<<sc<<" b: "<<-ss<<" c: "<<ss<<" d: "<<sc<<" tx: "<<tx<<" ty: "<<ty<<endl;
    return Affine(img, sc,-ss,ss,sc,tx,ty,out_size,out_size);
}

cv::Mat face_align(cv::Mat img, cv::Point* lmks, cv::Point* standard_lmks, int out_size, int lmk_num=5){
    //Affine调用的库函数，仅此而已的区别，而且似乎并没有自己实现的快多少
    cv::Mat X = cv::Mat::zeros(lmk_num*2, 4, CV_64FC1);
    cv::Mat Y = cv::Mat::zeros(lmk_num*2, 1, CV_64FC1);
    cv::Mat A;
    for(int i=0;i<lmk_num;i++){
        Y.at<double>(i*2,0) = standard_lmks[i].x;
        Y.at<double>(i*2+1,0) = standard_lmks[i].y;
        X.at<double>(i*2,0) = lmks[i].x;
        X.at<double>(i*2,1) = -lmks[i].y;
        X.at<double>(i*2,2) = 1;
        X.at<double>(i*2,3) = 0;
        X.at<double>(i*2+1,0) = lmks[i].y;
        X.at<double>(i*2+1,1) = lmks[i].x;
        X.at<double>(i*2+1,2) = 0;
        X.at<double>(i*2+1,3) = 1;
    }
    A = (X.t()*X).inv()*X.t()*Y;
    double sc, ss, tx, ty;
    sc = A.at<double>(0,0);
    ss = A.at<double>(1,0);
    tx = A.at<double>(2,0);
    ty = A.at<double>(3,0);
    cout<<"align_matrix: a: "<<sc<<" b: "<<-ss<<" c: "<<ss<<" d: "<<sc<<" tx: "<<tx<<" ty: "<<ty<<endl;
    double Mm[2][3]={
        {sc,-ss,tx},
        {ss,sc,ty},
    };
    cv::Mat M(2,3,CV_64FC1,Mm);
    cv::Mat out;
    cv::warpAffine(img, out, M, cv::Size(out_size,out_size));
    cv::Mat out_ = out.clone();
    return out_;
}
int main(){
    cv::Mat img = cv::imread("../imgs/face.jpg", cv::IMREAD_COLOR);
    cv::Point lmks[5];
    cv::Point slmks[5];
    lmks[0] = cv::Point(83,202);
    lmks[1] = cv::Point(131,159);
    lmks[2] = cv::Point(135,223);
    lmks[3] = cv::Point(141,250);
    lmks[4] = cv::Point(172,224);
    slmks[0] = cv::Point(37,50);
    slmks[1] = cv::Point(73,50);
    slmks[2] = cv::Point(56,71);
    slmks[3] = cv::Point(42,93);
    slmks[4] = cv::Point(71,93); // 112*112模板的标注landmark
    cv::Mat out = Face_Align_Affine(img, lmks, slmks, 112);
    //cv::Mat out = face_align(img, lmks, slmks, 112);
    cv::imshow("align_face", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
