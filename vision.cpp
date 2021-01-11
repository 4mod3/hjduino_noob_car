#include "vision.h"

using namespace std;
using namespace cv;

// int main ()
// {
// // VideoCapture capture(0);
// // if(!capture.isOpened()) 
// //      return -1;
// // while(1)
// // {
	
// 	Mat frame, gray, image, dst;
// 	//capture>>frame;
//     // circle(frame, Point(320, 240), 5, Scalar(0,255, 0));
//     // putText(frame, to_string(frame.at<Vec3b>(320, 240)[0])+","+to_string(frame.at<Vec3b>(320, 240)[1])+","+to_string(frame.at<Vec3b>(320, 240)[2]), Point(320, 240), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 1, 8, 0);
//     //imshow("raw", frame);
// 	cvtColor(frame, gray, COLOR_BGR2GRAY);
// 	threshold(gray, image, 0, 255, THRESH_OTSU);
// 	long sum = 0, count = 0;
// 	for(int i=0;i<image.cols;i++){
// 		if(image.at<int>(240, i) == 0){
// 			count += 1;
// 			sum += i;
// 		}
// 	}
// 	int center = count > 0?sum/count:320;
// 	cvtColor(image, dst, COLOR_GRAY2BGR);
// 	circle(dst, Point(center, 240), 5, Scalar(0,0,255));
// 	circle(dst, Point(320, 240), 5, Scalar(0,255, 0));

//     //if(center>340)trunRight();
//     //else if(center<300>)trunLeft();

// 	bool isStop = signDetect(frame, "stop", 8, 10000);
// 	//if(isStop)Stop();
//     bool isSound = signDetect(frame, "sound", 8, 10000);
// 	//if(isSound)Sound();
//     //printf("stop:%i, sound:%i\n", isStop, isSound);

// 	//imshow("Result", dst);
// 	//if(waitKey(30) == 27)break;
// // }
// 	return 0;
// }

int visualControlByCenter(Mat& frame)
{
    Mat gray, image;

    cvtColor(frame, gray, COLOR_BGR2GRAY);
	threshold(gray, image, 0, 255, THRESH_OTSU);
	long sum = 0, count = 0;
	for(int i=0;i<image.cols;i++){
		if(image.at<int>(240, i) == 0){
			count += 1;
			sum += i;
		}
	}

    return count > 0?sum/count:320;
}

int right_left_distence(Mat& image, int row_num)
{
    int middle_cols = image.cols / 2;
    int left_count=0, right_count=0;
    // line(image, Point(0, row_num), Point(image.cols-1, row_num), Scalar( 0, 0, 0 ), 3);
    //printf("%d\n", image.step[0]);

    for(int i = middle_cols-1; i>=0; i--)
    {
        //printf("%d\n", image.at<uchar>(row_num, i));
        if(image.at<uchar>(row_num, i) == 255){
            left_count++;
        }else{
            break;
        }
    }

    for(int j = middle_cols; j<image.cols; j++)
    {
        if(image.at<uchar>(row_num, j) == 255){
            right_count++;
        }else{
            break;
        }
    }
    // printf("%d, %d\n", left_count, right_count);
    return right_count - left_count;
}

bool signDetect(Mat& src, string signType,double epsilon, int minAcreage)
{
    Mat dst, mask;
	cvtColor(src, dst, COLOR_BGR2HSV);
    if(signType == "stop"){
        Mat mask1;
        inRange(dst, Scalar(0, 43, 46), Scalar(10, 255, 255), mask);
        inRange(dst, Scalar(156, 43, 46), Scalar(180, 255, 255), mask1);
        bitwise_or(mask, mask1, mask);
        // imshow("red", mask);
    }
	else if(signType == "sound"){
        inRange(dst, Scalar(100, 43, 46), Scalar(124, 255, 255), mask);
        // imshow("blue", mask);
    }
    
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); 
    morphologyEx(mask, mask, MORPH_OPEN, element); 
    morphologyEx(mask, mask, MORPH_CLOSE, element);

    //��Ե���?
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat canny_output;
    Canny(mask, canny_output, 1, 3, 7, true);  //Canny���?

    //��������
    Mat image = canny_output.clone();
    findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());

    
    if(signType == "stop"){
        for(int i=0;i<contours.size();i++){
            double acreage = contourArea(contours[i], true);
            if(acreage > minAcreage) { //���ɸ�?
                vector<Point> contourspoly;
                approxPolyDP(contours[i], contourspoly, epsilon, true);//��ָ�����ȱƽ����������?
                if(contourspoly.size() == 8)return true;
                // for(int j=0;j<contourspoly.size();j++){
                //     circle(src, contourspoly[j], 4, Scalar(0,0,255), -1);
                // }
            }
        }
        // imshow("stop",src);
    }
	else if(signType == "sound"){
        for(int i=0;i<contours.size();i++){
            double acreage = contourArea(contours[i], true);
            if(acreage > minAcreage) { //���ɸ�?   
                RotatedRect ell = fitEllipse(contours[i]);
                if(acreage/(pi*ell.size.width*ell.size.height) > 0.2){
                    return true;
                    // ellipse(src, ell, Scalar(0,0,255), 2);
                }
            }
        }
        // imshow("sound",src);
    }
    
    return false;
}
