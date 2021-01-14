#include "motion.h"

void run()     // 前进
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,80); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,80); 
}

void brake(int time)         //刹车，停�????
{
    softPwmWrite(1,0); //左轮stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //stop
	softPwmWrite(6,0); 
    delay(time * 100);//执�?�时间，�????以调�????  
}

void RotateLeft()         //左转()
{
    softPwmWrite(4,100); //左轮
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,0); 
	printf("RotateLeft\n");
	//delay(time * 300);
   // delay(time * 300);

}


void RotateRight()        //右转()
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,0); 
	softPwmWrite(6,100); //右轮
	softPwmWrite(5,0); 
	printf("RotateRight\n");
    //delay(time * 300);	//执�?�时间，�????以调�????
}

void turnLeft(float auto_fit)         //左转()
{
    softPwmWrite(4,0); //左轮
	softPwmWrite(1,20); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,400 + auto_fit); 
	printf("turnLeft\n");
	//delay(time * 300);
   // delay(time * 300);

}


void turnRight(float auto_fit)        //右转()
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,400 + auto_fit); 
	softPwmWrite(6,0); //右轮
	softPwmWrite(5,20); 
	printf("turnRight\n");
    //delay(time * 300);	//执�?�时间，�????以调�????
}

void back()          //后退
{
    softPwmWrite(4,80); //左轮back
	softPwmWrite(1,0); 
	softPwmWrite(6,80); //右轮back
  	softPwmWrite(5,0); 
    //delay(time *200);     //执�?�时间，�????以调�????
}

void visual_forward(int diff_right, int diff_left)
{
	if(diff_left > 0){
		diff_right = diff_right * 2;
		diff_left = diff_left * 2;
	}else{
		diff_left = diff_left * 2;
		diff_right = diff_right * 2;
	}
	softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,130-diff_left); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,130-diff_right); 
	printf("forword\n");
	printf("%d, %d\n", 130-diff_left, 130-diff_right);
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
// 	//�???�????�线避障 
// 		//有信号为LOW  没有信号为HIGH
// 	 	SR = digitalRead(RIGHT_BZ);//
// 	  	SL = digitalRead(LEFT_BZ);//
// 	    if (SL == LOW&&SR==LOW)
// 		{
// 	    	printf("BACK\n");  //前面有物体时小车后退500ms 再转�????
// 	    	back();
// 	    	delay(300);
// 	   		//后退500ms
// 	   		left();//左转400ms
// 	   		delay(601);
// 		}
		
// 	  	else if (SL == HIGH&&SR == LOW)
// 	  	{//左边有物�???? 
// 			printf("RIGHT\n");
// 			left();
		  
// 	  	}
	  	
// 		else if (SR == HIGH&&SL == LOW) 
// 		{//右边有物�???? 
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
//   		xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域，车子底板上L�????；没信号表明压在黑线上，车子底板上L�????
//  		xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域，车子底板上L�????；没信号表明压在黑线上，车子底板上L�????
 		
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
// 		{// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右�????  
			
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
