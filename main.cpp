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
			printf("BACK\n");  //前面有物体时小车后退500ms 再转
	   		back();
			delay(300);
			//后退500ms
			RotateLeft();//左转400ms
			delay(601);
		}
	  	else if (SL == HIGH&&SR == LOW)
	  	{//左边有物体
			printf("RIGHT\n");
			RotateLeft();
		 
	  	}
	  	
		else if (SR == HIGH&&SL == LOW) 
		{
			//右边有物体 
			printf("LEFT\n");
			RotateRight();
		}
}

bool edge_request()
{
	xunji_left = digitalRead(RIGHT_XJ);//有信号表明在白色区域
 	xunji_right = digitalRead(LEFT_XJ);//有信号表明在白色区域
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
			// 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右转
		
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
    bool distence_flag = true;
    bool edge_flag = true;
	bool continue_flag = false;
	int res[5] = {0};
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
	pinMode (7, OUTPUT);	//bee

    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);

	while(1)
	{
		clock_t time_start = clock();

		//寻迹传感器（边缘）使能，红外传感器（避障）使能
        distence_flag = false;
		edge_flag = false;
		// first scan
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

		//红外避障
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

		// 获取实时图像
		capture>>frame;
		capture.set(CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CAP_PROP_FRAME_HEIGHT, 480);

		// 图像预处理
		Mat gray, image;
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		threshold(gray, image, 40, 255, THRESH_BINARY);

		// 形态学滤波
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

		// 停止标签检测
		while(signDetect(frame, "stop", 8, 10000))
		{
			printf("Get Sign >> STOP\n");
			brake(5);
			capture>>frame;
		}

		// second.fifth scan
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

		// 蜂鸣器标志检测
		if(signDetect(frame, "sound", 8, 10000)){
			printf("Bee---");
			digitalWrite(7, true);
			delay(500);
			digitalWrite(7, false);
		}

		// 五根线采样
		res[0] = right_left_distence(image, 48);
		res[1] = right_left_distence(image, 144);
		res[2] = right_left_distence(image, 240);
		res[3] = right_left_distence(image, 288);
		res[4] = right_left_distence(image, 384);
		int mean_res = (res[0]+ res[1]+ res[2]+ res[3]+ res[4])/5;

		// 转换成pwm差值
		int diff_pwm_cyc = mean_res * 5 / 16;
		
		// 连通域预处理
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
		
		// 过滤较小面积的连通域, 去噪
		for(int i=1;i<num_con;i++){
			if(stats.at<int>(i, CC_STAT_AREA) > 1200){
				list.push_back(i);
			};
		}
		printf("num:%d\n", list.size());

		// 只有一个连通域，单边道路线
		if(list.size() == 1){
			printf("center: %lf\n", centroids.at<double>(list[0], 1));
			// 质心位置高
			if(centroids.at<double>(list[0], 1) < 45){
				visual_forward(diff_pwm_cyc, -diff_pwm_cyc);
			}
			else if(centroids.at<double>(list[0], 1) < 250){
				//质心位置中
				double angle = (double)stats.at<int>(list[0], CC_STAT_HEIGHT)/(double)stats.at<int>(list[0], CC_STAT_WIDTH);
				int bottom = stats.at<int>(list[0], CC_STAT_TOP) + stats.at<int>(list[0], CC_STAT_HEIGHT)-1;
				// 计算转向方向
				int x1 = 0, x2 = 0;
				for(int i=0;i<stats.at<int>(list[0], CC_STAT_WIDTH);i++){
					if(x1 == 0 && image.at<uchar>(stats.at<int>(list[0], CC_STAT_TOP), stats.at<int>(list[0], CC_STAT_LEFT)+i) == 255){
						x1 = stats.at<int>(list[0], CC_STAT_LEFT)+i;
					}
					if(x2 == 0 && image.at<uchar>(bottom, stats.at<int>(list[0], CC_STAT_LEFT)+i) == 255){
						x2 = stats.at<int>(list[0], CC_STAT_LEFT)+i;
					}
				}
				// 角度逻辑
				printf("angle: %f\n", angle);
				if(angle > 1.2){
					visual_forward(diff_pwm_cyc, -diff_pwm_cyc);
				}
				else if(x1 < x2){
					// 左转
					if(angle >= 1){
						turnLeft(0);
					}
					else{
						turnLeft(1.5*log(angle)<-100?100:-1.5*log(angle));
					}
				}
				else{
					// 右转
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
				// 质心位置低
				int bottom = stats.at<int>(list[0], CC_STAT_TOP) + stats.at<int>(list[0], CC_STAT_HEIGHT)-1;
				// 原地转向的方向
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
					// 左转
					RotateLeft();
				}
				else{
					// 右转
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
			// 无道路线
			back();
		}
		else{
			// 两条道路线
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