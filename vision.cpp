#include "vision.h"

using namespace std;
using namespace cv;

int right_left_distence(Mat& image, int row_num)
{
    int middle_cols = image.cols / 2;
    int left_count=0, right_count=0;

    // 左边白色块数量
    for(int i = middle_cols-1; i>=0; i--)
    {
        if(image.at<uchar>(row_num, i) == 255){
            left_count++;
        }else{
            break;
        }
    }

    // 右边白色块数量
    for(int j = middle_cols; j<image.cols; j++)
    {
        if(image.at<uchar>(row_num, j) == 255){
            right_count++;
        }else{
            break;
        }
    }
    // 返回右边-左边差
    return right_count - left_count;
}

bool signDetect(Mat& src, string signType,double epsilon, int minAcreage)
{
    Mat dst, mask;
    // RGB to HSV
	cvtColor(src, dst, COLOR_BGR2HSV);
    // 检测初始化
    if(signType == "stop"){
        Mat mask1;
        inRange(dst, Scalar(0, 43, 46), Scalar(10, 255, 255), mask);
        inRange(dst, Scalar(156, 43, 46), Scalar(180, 255, 255), mask1);
        bitwise_or(mask, mask1, mask);
    }
	else if(signType == "sound"){
        inRange(dst, Scalar(100, 43, 46), Scalar(124, 255, 255), mask);
    }
    
    // 形态学滤波
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); 
    morphologyEx(mask, mask, MORPH_OPEN, element);
    morphologyEx(mask, mask, MORPH_CLOSE, element);

    // Canny 边缘提取
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat canny_output;
    Canny(mask, canny_output, 1, 3, 7, true);

    // 轮廓提取
    Mat image = canny_output.clone();
    findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());

    if(signType == "stop"){
        for(int i=0;i<contours.size();i++){
            double acreage = contourArea(contours[i], true);
            // 面积筛除
            if(acreage > minAcreage) {
                vector<Point> contourspoly;
                // 多边形近似
                approxPolyDP(contours[i], contourspoly, epsilon, true);
                // 近似度高（8个端点）
                if(contourspoly.size() == 8)return true;
            }
        }
    }
	else if(signType == "sound"){
        for(int i=0;i<contours.size();i++){
            double acreage = contourArea(contours[i], true);
            // 面积筛除
            if(acreage > minAcreage) {  
                // 椭圆近似
                RotatedRect ell = fitEllipse(contours[i]);
                // 面积近似度高，（80%）
                if(acreage/(pi*ell.size.width*ell.size.height) > 0.2){
                    return true;
                }
            }
        }
    } 
    return false;
}
