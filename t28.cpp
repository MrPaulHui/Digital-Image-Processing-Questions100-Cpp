//仿射变换——平移
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#include "common.h"
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR);
    cv::Mat out = Affine(img, 1,0,0,1,30,-30);
    cv::imshow("t28", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
