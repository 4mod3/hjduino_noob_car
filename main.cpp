#include <pthread.h>

#include "motion.h"
#include "vision.h"

using namespace std;
using namespace cv;

int SR;
int SL;
	
int xunji_left;
int xunji_right;

bool distence_request()
{
	SR = digitalRead(RIGHT_BZ);
	SL = digitalRead(LEFT_BZ);
	return !(SR && SL); // false if normal
}

void distence_handle()
{
	if (SL == LOW&&SR==LOW)
		{
			printf("BACK\n");  //前面有物体时小车后退500ms 再转�???
	   		back();
			delay(300);
			//后退500ms
			RotateLeft();//左转400ms
			delay(601);
		}
	  	else if (SL == HIGH&&SR == LOW)
	  	{//左边有物�??? 
			printf("RIGHT\n");
			RotateLeft();
		 
	  	}
	  	
		else if (SR == HIGH&&SL == LOW) 
		{
			//右边有物�??? 
			printf("LEFT\n");
			RotateRight();
		}
}

bool edge_request()
{
	xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域，车子底板上L�???；没信号表明压在黑线上，车子底板上L�???
 	xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域，车子底板上L�???；没信号表明压在黑线上，车子底板上L�???
 	return xunji_left || xunji_right; // false if normal
}

void edge_handle()
{
	if (xunji_left == HIGH&&xunji_right == LOW)
		{
			printf("RIGHT\n");
			back();
			delay(5);
			RotateRight ();
			delay(5);
		}
		else if (xunji_right == HIGH&&xunji_left == LOW)
		{
			// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右�???  
		
			printf("LEFT\n");
			back();
			delay(5);
			RotateLeft();
			delay(5);
		}
		else 
		{
			printf("STOP\n");
			brake(2);
		}
}

int main(int argc, char *argv[])
{

    float dis;
	
    bool distence_flag = true;
    bool edge_flag = true;
	bool continue_flag = false;
	int res[5] = {0};
	int center = 0;

	//get video
    VideoCapture capture(0);

	printf("\n");
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
		clock_t time_start = clock();

		//�??�???�测距和�???迹使�???
        distence_flag = false;
		edge_flag = false;
		while(edge_flag && edge_request())
		{
			edge_handle();
			printf("Mind Edge!!");
			continue_flag = true;
		}
		if(continue_flag){
			continue_flag = false;
			continue;
		}

		//distence_flag
		while(distence_flag && distence_request())
		{
			distence_handle();
			printf("Obstacle!");
			continue_flag = true;
		}
		if(continue_flag){
			continue_flag = false;
			continue;
		}

		// if(distence_flag == false)
		// {
		// 	//�??�???�线避障 
		// 	//有信号为LOW  没有信号为HIGH
	 	// 	SR = digitalRead(RIGHT_BZ);//
	  	// 	SL = digitalRead(LEFT_BZ);//
	    // 	if (SL == LOW&&SR==LOW)
		// 	{
	    // 		printf("BACK\n");  //前面有物体时小车后退500ms 再转�???
	    // 		back();
	    // 		delay(300);
	   	// 		//后退500ms
	   	// 		RotateLeft();//左转400ms
	   	// 		delay(601);
		// 	}
	  	// 	else if (SL == HIGH&&SR == LOW)
	  	// 	{//左边有物�??? 
		// 		printf("RIGHT\n");
		// 		RotateLeft();
		  
	  	// 	}
	  	
		// 	else if (SR == HIGH&&SL == LOW) 
		// 	{
		// 		//右边有物�??? 
		// 		printf("LEFT\n");
		// 		RotateRight();
		// 	}
		
	  	// 	else if(SR == HIGH&&SL == HIGH)
		// 	{// 前面没有物体 前进
		// 	 	printf("GO\n");
		// 	 	distence_flag = true;
	    // 	}
		// }
	
		// if(edge_flag == false)
		// {
		// 	//寻迹  此程序为赛道在两边的 
		// 	//有信号为LOW  没有信号为HIGH
		// 	//LOW 白色 high 黑色 
  		// 	xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域，车子底板上L�???；没信号表明压在黑线上，车子底板上L�???
 		// 	xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域，车子底板上L�???；没信号表明压在黑线上，车子底板上L�???
 		
 		// 	if (xunji_left == LOW&&xunji_right==LOW)
		// 	{
   		// 		printf("GO\n");
		//    		edge_flag = true;
  		// 	}
		// 	else if (xunji_left == HIGH&&xunji_right == LOW)
		// 	{
		// 		printf("RIGHT\n");
		// 		back();
		// 		delay(30);
		// 		RotateRight ();
		// 		delay(80);
		// 	}
		// 	else if (xunji_right == HIGH&&xunji_left == LOW)
		// 	{
		// 		// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右�???  
			
		// 		printf("LEFT\n");
		// 		back();
		// 		delay(30);
		// 		RotateLeft();
		// 		delay(80);
		// 	}
		// 	else 
		// 	{
		// 		printf("STOP\n");
   		// 		brake(2);
		// 	}
		// }

		// capture>>frame;
		// imshow("raw", frame);

		// printf("%i, %i\n", distence_flag, edge_flag);
			//使用视�?�进行控�???
			//在本段逻辑之前�???进�?�控制操�???
			// printf("visual control\n");
		// if(edge_flag && distence_flag){	
			capture>>frame;
			capture.set(CAP_PROP_FRAME_WIDTH, 640);
			capture.set(CAP_PROP_FRAME_HEIGHT, 480);
			Mat gray, image;
    		cvtColor(frame, gray, COLOR_BGR2GRAY);
			threshold(gray, image, 40, 255, THRESH_BINARY);

    		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5)); 
			morphologyEx(image, image, MORPH_OPEN, element); 
			element = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));
    		morphologyEx(image, image, MORPH_CLOSE, element);

			// second scan
			while(edge_flag && edge_request())
			{
				edge_handle();
				printf("Mind Edge!!");
				continue_flag = true;
			}
			if(continue_flag){
				continue_flag = false;
				continue;
			}

			// �???�???检测停止标�???
			while(signDetect(frame, "stop", 8, 10000))
			{
				printf("Get Sign >> STOP\n");
   				brake(5);
				capture>>frame;
				//delay(1000);
			}

			// second.five scan
			while(edge_flag && edge_request())
			{
				edge_handle();
				printf("Mind Edge!!");
				continue_flag = true;
			}
			if(continue_flag){
				continue_flag = false;
				continue;
			}

			if(signDetect(frame, "sound", 8, 10000)){
				//蜂鸣�???
				printf("Bee---");
				digitalWrite(7, true);
				delay(500);
				digitalWrite(7, false);
			}

			res[0] = right_left_distence(image, 48);
			res[1] = right_left_distence(image, 144);
			res[2] = right_left_distence(image, 240);
			res[3] = right_left_distence(image, 288);
			res[4] = right_left_distence(image, 384);
			int mean_res = (res[0]+ res[1]+ res[2]+ res[3]+ res[4])/5;
			// printf("%d\n", mean_res);

			int diff_pwm_cyc = mean_res * 5 / 16;
			// printf("%d\n", diff_pwm_cyc);
			// visual_forward(-diff_pwm_cyc, diff_pwm_cyc);
			Mat labels, stats, centroids;
			image = image(Rect(0, 0, 640, 380));
			bitwise_not(image, image);
			int num_con = connectedComponentsWithStats(image, labels, stats, centroids);
			vector<int> list;
			
			// third scan
			while(edge_flag && edge_request())
			{
				edge_handle();
				printf("Mind Edge!!");
				continue_flag = true;
			}
			if(continue_flag){
				continue_flag = false;
				continue;
			}
			
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
				printf("center: %lf\n", centroids.at<double>(list[0], 1));
				if(centroids.at<double>(list[0], 1) < 45){
					visual_forward(diff_pwm_cyc, -diff_pwm_cyc);
				}
				else if(centroids.at<double>(list[0], 1) < 250){
					double angle = (double)stats.at<int>(list[0], CC_STAT_HEIGHT)/(double)stats.at<int>(list[0], CC_STAT_WIDTH);
					int bottom = stats.at<int>(list[0], CC_STAT_TOP) + stats.at<int>(list[0], CC_STAT_HEIGHT)-1;
					int x1 = 0, x2 = 0;
					for(int i=0;i<stats.at<int>(list[0], CC_STAT_WIDTH);i++){
						if(x1 == 0 && image.at<uchar>(stats.at<int>(list[0], CC_STAT_TOP), stats.at<int>(list[0], CC_STAT_LEFT)+i) == 255){
							x1 = stats.at<int>(list[0], CC_STAT_LEFT)+i;
						}
						if(x2 == 0 && image.at<uchar>(bottom, stats.at<int>(list[0], CC_STAT_LEFT)+i) == 255){
							x2 = stats.at<int>(list[0], CC_STAT_LEFT)+i;
							// printf("update x2: %d\n", x2);
						}
					}
					// circle(frame, Point(x2, bottom), 5, Scalar(0, 0, 255));
					// imshow("frame", frame);
					printf("angle: %f\n", angle);
					if(angle > 1.2){
						visual_forward(diff_pwm_cyc, -diff_pwm_cyc);
					}
					else if(x1 < x2){
						if(angle >= 1){
							turnLeft(0);
						}
						else{
							turnLeft(1.5*log(angle)<-100?100:-1.5*log(angle));
						}
					}
					else{
						if(angle >= 1){
							turnRight(0);
						}
						else{
							turnRight(1.5*log(angle)<-100?100:-1.5*log(angle));
						}
					}

					// fourth scan
					while(edge_flag && edge_request())
					{
						edge_handle();
						printf("Mind Edge!!");
						continue_flag = true;
					}
					if(continue_flag){
						continue_flag = false;
						continue;
					}
				}
				else{
					int bottom = stats.at<int>(list[0], CC_STAT_TOP) + stats.at<int>(list[0], CC_STAT_HEIGHT)-1;
					int x1 = 0, x2 = 0;
					for(int i=0;i<stats.at<int>(list[0], CC_STAT_WIDTH);i++){
						if(x1 == 0 && image.at<uchar>(stats.at<int>(list[0], CC_STAT_TOP), stats.at<int>(list[0], CC_STAT_LEFT)+i) == 255){
							x1 = stats.at<int>(list[0], CC_STAT_LEFT)+i;
						}
						if(x2 == 0 && image.at<uchar>(bottom, stats.at<int>(list[0], CC_STAT_LEFT)+i) == 255){
							x2 = stats.at<int>(list[0], CC_STAT_LEFT)+i;
						}
					}
					printf("x1: %d, x2: %d\n", x1, x2);
					if(x1 < x2){
						RotateLeft();
					}
					else{
						RotateRight();
					}

					// fifith scan
					while(edge_flag && edge_request())
					{
						edge_handle();
						printf("Mind Edge!!");
						continue_flag = true;
					}
					if(continue_flag){
						continue_flag = false;
						continue;
					}
				}
			}
			else if(list.size() == 0){
				back();
			}
			else{
				visual_forward(diff_pwm_cyc, -diff_pwm_cyc);
				// sixth scan
					while(edge_flag && edge_request())
					{
						edge_handle();
						printf("Mind Edge!!");
						continue_flag = true;
					}
					if(continue_flag){
						continue_flag = false;
						continue;
					}
			}
			imshow("image", image);
			waitKey(30);
		clock_t time_end = clock();
		printf("%lf ms\n", 1000*(time_end-time_start)/(double)CLOCKS_PER_SEC);
	}

  return 0;

}