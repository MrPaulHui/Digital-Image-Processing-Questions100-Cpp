/*tutorial*/
// read and show image
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <typeinfo>
using namespace std;

int main(){
    cv::Mat img = cv::imread("../imgs/imori.jpg", cv::IMREAD_COLOR); //注意这里的路径，因为可执行文件是在build文件夹下的，所以路径起点是build文件夹下
    int w = img.rows;
    int h = img.cols;
    cout<<w<<h<<endl;
    cout<<img.at<cv::Vec3b>(30,20)<<endl; //show the value of a point in image
    cout<<typeid(img.at<cv::Vec3b>(30,20)).name()<<endl;
    auto value = img.at<cv::Vec3b>(30,20); //这个变量的类型有些复杂，还是先直接用auto吧
    cout<<value<<endl;
    cv::imshow("sam", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
