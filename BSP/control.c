
#include "control.h"
#include "PWM.h"
#include "arm_math.h"
#define lat_base 30.768
#define lon_base 103.985
int16_t volatile linear, angle;
uint16_t  yun_yaw_set = yaw_set_type, yun_pitch_set = pitch_set_type;
int8_t volatile yun_yaw, yun_pitch;
uint8_t volatile connected_flag;
 float dir_deg;//方向

//全局变量
// uint16_t Stop_Counter;
// uint16_t motor_m=1490;
// uint16_t servo_m=1530;
// int16_t Servo_set, Motor_set;
//double pA6[2]={103.9859285,30.7681785};
float Dist;//距离
float Dire;//方向
extern uint8_t pos_des_ptr;//目标次序
extern double pos_now[3];//纬度、经度、水平精度	
extern double pos_des[100];

void Move_Control()
{

	//get_dire_dist();//更新相对目的地的方向和距离

			PWM_servo_set(servo_set_type+angle); 
			PWM_motor_set(motor_set_type+linear/5);
			
			yun_pitch_set += yun_pitch*3;
			if (yun_pitch_set > pitch_set_max)
				yun_pitch_set=pitch_set_max;
			else if (yun_pitch_set < pitch_set_min)
				yun_pitch_set=pitch_set_min;
			
			yun_yaw_set += yun_yaw*3;
			if (yun_yaw_set > yaw_set_max)
				yun_yaw_set=yaw_set_max;
			else if (yun_yaw_set < yaw_set_min)
				yun_yaw_set=yaw_set_min;
			
			PWM_pitch_set(yun_pitch_set); 
			PWM_yaw_set(yun_yaw_set);
	
}
void Move_Stop()
{
	PWM_servo_set(servo_set_type); 
	PWM_motor_set(motor_set_type);
}

void get_dire_dist()
{
	float pos_des_lat;
	float pos_des_lon;
	float pos_now_lat;
	float pos_now_lon;
// 	pos_des_lat=(pos_des[pos_des_ptr*2]-lat_base)*1000;
// 	pos_des_lon=(pos_des[pos_des_ptr*2+1]-lon_base)*1000;
// 	pos_now_lat=(pos_now[0]-lat_base)*1000;
// 	pos_now_lon=(pos_now[1]-lon_base)*1000;
// 	arm_sqrt_f32((pos_des_lat-pos_now_lat)*(pos_des_lat-pos_now_lat)+(pos_des_lon-pos_now_lon)*(pos_des_lon-pos_now_lon),&Dist);
// 	Dist=Dist/180*PI*6371.935f;//距离 单位m
// 	Dire=(atan((pos_des_lon-pos_now_lon)/(pos_des_lat-pos_now_lat)))/PI*180-dir_deg;
	//Dist=sqrt((pos_des[pos_des_ptr*2]-pos_now[0])*(pos_des[pos_des_ptr*2]-pos_now[0])+(pos_des[pos_des_ptr*2+1]-pos_now[1])*(pos_des[pos_des_ptr*2+1]-pos_now[1]))*6371935;
	//Dire=(atan((pos_des[pos_des_ptr*2+1]-pos_now[1])/(pos_des[pos_des_ptr*2]-pos_now[0])))/3.14*180-dir_deg;
	//dist=sqrt((pA[0]-p0[0])^2+(pA[1]-p0[1])^2)*6378100;
// 		*dist=sqrt((pAA[0]-p0[0])*(pAA[0]-p0[0])+(pAA[1]-p0[1])*(pAA[1]-p0[1]))*6378100;
// 		return (atan((pAA[0]-p0[0])/(pAA[1]-p0[1])))/3.14*180-dir_deg;
}
