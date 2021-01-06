#include "motion.h"

void run()     // 前进
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,250); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,250); 
}

void brake(int time)         //刹车，停车
{
    softPwmWrite(1,0); //左轮stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //stop
	softPwmWrite(6,0); 
    delay(time * 100);//执行时间，可以调整  
}

void left()         //左转()
{
    softPwmWrite(4,250); //左轮
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,250); 
	//delay(time * 300);
   // delay(time * 300);

}


void right()        //右转()
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,250); 
	softPwmWrite(6,250); //右轮
	softPwmWrite(5,0); 
    //delay(time * 300);	//执行时间，可以调整
}



void back()          //后退
{
    softPwmWrite(4,250); //左轮back
	softPwmWrite(1,0); 
	softPwmWrite(6,250); //右轮back
  	softPwmWrite(5,0); 
    //delay(time *200);     //执行时间，可以调整
}

void visual_forward(int diff_right, int diff_left)
{
	softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,250-diff_left); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,250-diff_right); 
}


// int main(int argc, char *argv[])
// {

//     float dis;

// 	int SR;
// 	int SL;
	
// 	int xunji_left;
// 	int xunji_right;
	
//     /*RPI*/
//     wiringPiSetup();
//     /*WiringPi GPIO*/
//     pinMode (1, OUTPUT);	//IN1
//     pinMode (4, OUTPUT);	//IN2
//     pinMode (5, OUTPUT);	//IN3
//     pinMode (6, OUTPUT);	//IN4

//     softPwmCreate(1,1,500);   
//     softPwmCreate(4,1,500);
//     softPwmCreate(5,1,500);
//     softPwmCreate(6,1,500);

// 	while(1)
// 	{
// 	//红外线避障 
// 		//有信号为LOW  没有信号为HIGH
// 	 	SR = digitalRead(RIGHT_BZ);//
// 	  	SL = digitalRead(LEFT_BZ);//
// 	    if (SL == LOW&&SR==LOW)
// 		{
// 	    	printf("BACK\n");  //前面有物体时小车后退500ms 再转弯
// 	    	back();
// 	    	delay(300);
// 	   		//后退500ms
// 	   		left();//左转400ms
// 	   		delay(601);
// 		}
		
// 	  	else if (SL == HIGH&&SR == LOW)
// 	  	{//左边有物体 
// 			printf("RIGHT\n");
// 			left();
		  
// 	  	}
	  	
// 		else if (SR == HIGH&&SL == LOW) 
// 		{//右边有物体 
// 			printf("LEFT\n");
// 			right();
// 		}
		
// 	  	else if(SR == HIGH&&SL == HIGH)
// 		{// 前面没有物体 前进
// 			 printf("GO\n");
// 			 run();
// 	    }
// 	//寻迹  此程序为赛道在两边的 
// 		//有信号为LOW  没有信号为HIGH
// 		//LOW 白色 high 黑色 
//   		xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
//  		xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
 		
//  		if (xunji_left == LOW&&xunji_right==LOW)
// 		{
//    			printf("GO\n");
// 		   	run();
//   		}
// 		else if (xunji_left == HIGH&&xunji_right == LOW)
// 		{
// 			printf("RIGHT\n");
// 			right ();
// 			delay(500);
// 		}
// 		else if (xunji_right == HIGH&&xunji_left == LOW)
// 		{// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右转  
			
// 			printf("LEFT\n");
// 			left();
// 			delay(500);
// 		}
// 		else 
// 		{
// 			printf("STOP\n");
//    			brake(2);
// 		}
// 	}

 
//   return 0;

// }
