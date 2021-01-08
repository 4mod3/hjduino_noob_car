#include <pthread.h>

#include "motion.h"
#include "vision.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    float dis;

	int SR;
	int SL;
	
	int xunji_left;
	int xunji_right;
	
    bool distence_flag = true;
    bool edge_flag = true;
	int res[5] = {0};
	int center = 0;
	int count = 0;

	//get video
    VideoCapture capture(0);
	Mat frame;
    if(!capture.isOpened()) 
    	return -1;

    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
	pinMode (7, OUTPUT);	//bee

    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);

	while(1)
	{
		//�?�??�测距和�??迹使�??
        distence_flag = true;
		edge_flag = true;

		if(distence_flag == false)
		{
			//�?�??�线避障 
			//有信号为LOW  没有信号为HIGH
	 		SR = digitalRead(RIGHT_BZ);//
	  		SL = digitalRead(LEFT_BZ);//
	    	if (SL == LOW&&SR==LOW)
			{
	    		printf("BACK\n");  //前面有物体时小车后退500ms 再转�??
	    		back();
	    		delay(300);
	   			//后退500ms
	   			RotateLeft();//左转400ms
	   			delay(601);
			}
	  		else if (SL == HIGH&&SR == LOW)
	  		{//左边有物�?? 
				printf("RIGHT\n");
				RotateLeft();
		  
	  		}
	  	
			else if (SR == HIGH&&SL == LOW) 
			{
				//右边有物�?? 
				printf("LEFT\n");
				RotateRight();
			}
		
	  		else if(SR == HIGH&&SL == HIGH)
			{// 前面没有物体 前进
			 	printf("GO\n");
			 	distence_flag = true;
	    	}
		}
	
		if(edge_flag == false)
		{
			//寻迹  此程序为赛道在两边的 
			//有信号为LOW  没有信号为HIGH
			//LOW 白色 high 黑色 
  			xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域，车子底板上L�??；没信号表明压在黑线上，车子底板上L�??
 			xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域，车子底板上L�??；没信号表明压在黑线上，车子底板上L�??
 		
 			if (xunji_left == LOW&&xunji_right==LOW)
			{
   				printf("GO\n");
		   		edge_flag = true;
  			}
			else if (xunji_left == HIGH&&xunji_right == LOW)
			{
				printf("RIGHT\n");
				back();
				delay(30);
				RotateRight ();
				delay(80);
			}
			else if (xunji_right == HIGH&&xunji_left == LOW)
			{
				// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右�??  
			
				printf("LEFT\n");
				back();
				delay(30);
				RotateLeft();
				delay(80);
			}
			else 
			{
				printf("STOP\n");
   				brake(2);
			}
		}

		// capture>>frame;
		// imshow("raw", frame);

		// printf("%i, %i\n", distence_flag, edge_flag);
		if((distence_flag && edge_flag == true))
		{
			//使用视�?�进行控�??
			//在本段逻辑之前�??进�?�控制操�??
			// printf("visual control\n");
			capture>>frame;

			Mat gray, image;
    		cvtColor(frame, gray, COLOR_BGR2GRAY);
			threshold(gray, image, 0, 255, THRESH_OTSU);

    		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5)); 
    		morphologyEx(image, image, MORPH_OPEN, element); 
    		morphologyEx(image, image, MORPH_CLOSE, element);

			// �??�??检测停止标�??
			// while(signDetect(frame, "stop", 8, 10000))
			// {
			// 	printf("Get Sign >> STOP\n");
   			// 	brake(5);
			// 	capture>>frame;
			// 	//delay(1000);
			// }

			// if(signDetect(frame, "sound", 8, 10000)){
			// 	//蜂鸣�??
			// 	printf("Bee---");
			// 	digitalWrite(7, true);
			// 	delay(2000);
			// 	digitalWrite(7, false);
			// }

			res[0] = right_left_distence(image, 240);
			res[1] = right_left_distence(image, 260);
			res[2] = right_left_distence(image, 280);
			res[3] = right_left_distence(image, 300);
			res[4] = right_left_distence(image, 320);
			int mean_res = (res[0]+ res[1]+ res[2]+ res[3]+ res[4])/5;
			// printf("%d\n", mean_res);

			int diff_pwm_cyc = mean_res * 5 / 16;
			// printf("%d\n", diff_pwm_cyc);
			// visual_forward(-diff_pwm_cyc, diff_pwm_cyc);
			Mat labels, stats, centroids;
			image = image(Rect(0, 0, 640, 400));
			bitwise_not(image, image);
			int num_con = connectedComponentsWithStats(image, labels, stats, centroids);
			vector<int> list;
			
			
			// cvtColor(image, image, COLOR_GRAY2BGR);
			for(int i=1;i<num_con;i++){
				// printf("%f, %f\n", centroids.at<double>(i, 0), centroids.at<double>(i, 1));
				if(stats.at<int>(i, CC_STAT_AREA) > 1200){
					list.push_back(i);
				};
				// circle(image, Point(int(centroids.at<double>(i, 0)), int(centroids.at<double>(i, 1))), 1, Scalar(0, 0, 255));
			}
			printf("num:%d\n", list.size());
			if(list.size() == 1){
				if(centroids.at<double>(1, 1) < 80){
					visual_forward(-diff_pwm_cyc, diff_pwm_cyc);
				}
				else if(centroids.at<double>(1, 1) < 180){
					double angle = stats.at<int>(1, CC_STAT_HEIGHT)/stats.at<int>(1, CC_STAT_WIDTH);
					int bottom = stats.at<int>(1, CC_STAT_TOP) + stats.at<int>(1, CC_STAT_HEIGHT)-1;
					int x1 = 0, x2 = 0;
					for(int i=0;i<stats.at<int>(1, CC_STAT_WIDTH);i++){
						if(x1 == 0 && image.at<uchar>(stats.at<int>(1, CC_STAT_TOP), stats.at<int>(1, CC_STAT_LEFT)+i) == 0){
							x1 = stats.at<int>(1, CC_STAT_LEFT)+i;
						}
						if(x2 == 0 && image.at<uchar>(bottom, stats.at<int>(1, CC_STAT_LEFT)+i) == 0){
							x2 = stats.at<int>(1, CC_STAT_LEFT)+i;
						}
					}
					if(x1 < x2){
						if(angle > 0.625){
							RotateLeft();
						}
						else{
							turnLeft();
						}
					}
					else{
						if(angle > 0.625){
							RotateRight();
						}
						else{
							turnRight();
						}
					}
				}
				else{
					int bottom = stats.at<int>(1, CC_STAT_TOP) + stats.at<int>(1, CC_STAT_HEIGHT);
					int x1, x2;
					for(int i=0;i<stats.at<int>(1, CC_STAT_WIDTH);i++){
						if(x1 == 0 && image.at<uchar>(stats.at<int>(1, CC_STAT_TOP), stats.at<int>(1, CC_STAT_LEFT)+i) == 0){
							printf("%d, %d",stats.at<int>(1, CC_STAT_LEFT),  stats.at<int>(1, CC_STAT_LEFT)+i);
							x1 = stats.at<int>(1, CC_STAT_LEFT)+i;
						}
						if(x2 == 0 && image.at<uchar>(bottom, stats.at<int>(1, CC_STAT_LEFT)+i) == 0){
							x2 = stats.at<int>(1, CC_STAT_LEFT)+i;
						}
					}
					if(x1 < x2){
						RotateLeft();
					}
					else{
						RotateRight();
					}
				}
			}
			else{
				visual_forward(-diff_pwm_cyc, diff_pwm_cyc);
			}
			delay(50);
			imshow("image", image);
			// delay(20);
			// center = visualControlByCenter(frame);
			// printf("%d", center);
			// if(center>340){
			// 	diff_right = (int)(0.6*(center - 340));
			// 	diff_left = 0;
			// }else if(center<300){
			// 	diff_left = (int)(0.6*(300 - center));
			// 	diff_right = 0;
			// }else{
			// 	diff_left = 0;
			// 	diff_right = 0;
			// }
			// run();
			// visual_forward(diff_right, diff_left);
			//delay(500);
			count = 0;
		}
		count++;
	}

  return 0;

}