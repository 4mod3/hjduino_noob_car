#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

#define pi 3.1415926

using namespace std;
using namespace cv;

extern bool signDetect(Mat& src, string signType, double epsilon, int minAcreage);
extern int right_left_distence(Mat& frame, int row_num);