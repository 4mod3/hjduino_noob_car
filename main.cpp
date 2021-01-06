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
	
    bool distence_flag = false;
    bool edge_flag = false;
	int diff_left = 0;
	int diff_right = 0;
	int center = 0;

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
	pinMode (7, OUTPUT);	//IN4

    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);

	while(1)
	{

        distence_flag = false;
		edge_flag = false;

	//红外线避障 
		//有信号为LOW  没有信号为HIGH
	 	SR = digitalRead(RIGHT_BZ);//
	  	SL = digitalRead(LEFT_BZ);//
	    if (SL == LOW&&SR==LOW)
		{
	    	printf("BACK\n");  //前面有物体时小车后退500ms 再转弯
	    	back();
	    	delay(300);
	   		//后退500ms
	   		left();//左转400ms
	   		delay(601);
		}
		
	  	else if (SL == HIGH&&SR == LOW)
	  	{//左边有物体 
			printf("RIGHT\n");
			left();
		  
	  	}
	  	
		else if (SR == HIGH&&SL == LOW) 
		{//右边有物体 
			printf("LEFT\n");
			right();
		}
		
	  	else if(SR == HIGH&&SL == HIGH)
		{// 前面没有物体 前进
			 printf("GO\n");
			 distence_flag = true;
	    }
	//寻迹  此程序为赛道在两边的 
		//有信号为LOW  没有信号为HIGH
		//LOW 白色 high 黑色 
  		xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
 		xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
 		
 		if (xunji_left == LOW&&xunji_right==LOW)
		{
   			printf("GO\n");
		   	edge_flag = true;
  		}
		else if (xunji_left == HIGH&&xunji_right == LOW)
		{
			printf("RIGHT\n");
			right ();
			delay(500);
		}
		else if (xunji_right == HIGH&&xunji_left == LOW)
		{// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右转  
			
			printf("LEFT\n");
			left();
			delay(500);
		}
		else 
		{
			printf("STOP\n");
   			brake(2);
		}

		// capture>>frame;
		// imshow("raw", frame);

		printf("%i, %i\n", distence_flag, edge_flag);
		if(distence_flag && edge_flag == true)
		{
			//使用视觉进行控制
			//在本段逻辑之前未进行控制操作
			printf("visual control\n");
			capture>>frame;
			// 循环检测停止标记
			while(signDetect(frame, "stop", 8, 10000))
			{
				printf("Get Sign >> STOP\n");
   				brake(5);
				capture>>frame;
				//delay(1000);
			}

			if(signDetect(frame, "sound", 8, 10000)){
				//蜂鸣器
				printf("Bee---");
				digitalWrite(7, true);
				delay(2000);
				digitalWrite(7, false);
			}

			center = visualControlByCenter(frame);
			if(center>340){
				diff_right = (int)(0.6*(center - 340));
				diff_left = 0;
			}else if(center<300){
				diff_left = (int)(0.6*(300 - center));
				diff_right = 0;
			}else{
				diff_left = 0;
				diff_right = 0;
			}
			
			visual_forward(diff_right, diff_left);
			//delay(500);
		}
		
	}

  return 0;

}