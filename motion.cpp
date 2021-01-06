#include "motion.h"

void run()     // ǰ��
{
    softPwmWrite(4,0); //����ǰ��
	softPwmWrite(1,250); 
	softPwmWrite(6,0); //����ǰ��
	softPwmWrite(5,250); 
}

void brake(int time)         //ɲ����ͣ��
{
    softPwmWrite(1,0); //����stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //stop
	softPwmWrite(6,0); 
    delay(time * 100);//ִ��ʱ�䣬���Ե���  
}

void left()         //��ת()
{
    softPwmWrite(4,250); //����
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //����ǰ��
	softPwmWrite(5,250); 
	//delay(time * 300);
   // delay(time * 300);

}


void right()        //��ת()
{
    softPwmWrite(4,0); //����ǰ��
	softPwmWrite(1,250); 
	softPwmWrite(6,250); //����
	softPwmWrite(5,0); 
    //delay(time * 300);	//ִ��ʱ�䣬���Ե���
}



void back()          //����
{
    softPwmWrite(4,250); //����back
	softPwmWrite(1,0); 
	softPwmWrite(6,250); //����back
  	softPwmWrite(5,0); 
    //delay(time *200);     //ִ��ʱ�䣬���Ե���
}

void visual_forward(int diff_right, int diff_left)
{
	softPwmWrite(4,0); //����ǰ��
	softPwmWrite(1,250-diff_left); 
	softPwmWrite(6,0); //����ǰ��
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
// 	//�����߱��� 
// 		//���ź�ΪLOW  û���ź�ΪHIGH
// 	 	SR = digitalRead(RIGHT_BZ);//
// 	  	SL = digitalRead(LEFT_BZ);//
// 	    if (SL == LOW&&SR==LOW)
// 		{
// 	    	printf("BACK\n");  //ǰ��������ʱС������500ms ��ת��
// 	    	back();
// 	    	delay(300);
// 	   		//����500ms
// 	   		left();//��ת400ms
// 	   		delay(601);
// 		}
		
// 	  	else if (SL == HIGH&&SR == LOW)
// 	  	{//��������� 
// 			printf("RIGHT\n");
// 			left();
		  
// 	  	}
	  	
// 		else if (SR == HIGH&&SL == LOW) 
// 		{//�ұ������� 
// 			printf("LEFT\n");
// 			right();
// 		}
		
// 	  	else if(SR == HIGH&&SL == HIGH)
// 		{// ǰ��û������ ǰ��
// 			 printf("GO\n");
// 			 run();
// 	    }
// 	//Ѱ��  �˳���Ϊ���������ߵ� 
// 		//���ź�ΪLOW  û���ź�ΪHIGH
// 		//LOW ��ɫ high ��ɫ 
//   		xunji_left = digitalRead(RIGHT_XJ);//���źű����ڰ�ɫ���򣬳��ӵװ���L����û�źű���ѹ�ں����ϣ����ӵװ���L��
//  		xunji_right = digitalRead(LEFT_XJ);//���źű����ڰ�ɫ���򣬳��ӵװ���L����û�źű���ѹ�ں����ϣ����ӵװ���L��
 		
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
// 		{// ��ѭ�����⴫����,��⵽�źţ���������ƫ����������ת  
			
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
