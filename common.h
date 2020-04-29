#pragma once
cv::Mat ToGray(cv::Mat img){
    int h = img.rows;
    int w = img.cols;
    cv::Mat out = cv::Mat::zeros(h, w, CV_8UC1);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            unsigned char r = img.at<cv::Vec3b>(i,j)[0];
            unsigned char g = img.at<cv::Vec3b>(i,j)[1];
            unsigned char b = img.at<cv::Vec3b>(i,j)[2];
            out.at<unsigned char>(i,j) = 0.2126 * r + 0.7152 * g + 0.0722 * b;
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
