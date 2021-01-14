#include "motion.h"

void run()     // 前进
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,150); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,150); 
}

void brake(int time)         //刹车
{
    softPwmWrite(1,0); //左轮stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //stop
	softPwmWrite(6,0); 
    delay(time * 100);//刹车时间
}

void RotateLeft()         //左转()
{
    softPwmWrite(4,220); //左轮
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,80); 
	printf("RotateLeft\n");

}


void RotateRight()        //右转()
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,80); 
	softPwmWrite(6,220); //右轮
	softPwmWrite(5,0); 
	printf("RotateRight\n");
}

void turnLeft(float auto_fit)         //左转()
{
    softPwmWrite(4,0); //左轮
	softPwmWrite(1,40); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,400 + auto_fit); 
	printf("turnLeft\n");

}


void turnRight(float auto_fit)        //右转()
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,400 + auto_fit); 
	softPwmWrite(6,0); //右轮
	softPwmWrite(5,40); 
	printf("turnRight\n");
}

void back()          //后退
{
    softPwmWrite(4,120); //左轮back
	softPwmWrite(1,0); 
	softPwmWrite(6,120); //右轮back
  	softPwmWrite(5,0); 
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
	softPwmWrite(1,130-diff_left>0?130-diff_left:0); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,130-diff_right>0?130-diff_right:0); 
	printf("forword\n");
	printf("%d, %d\n", 130-diff_left, 130-diff_right);
}