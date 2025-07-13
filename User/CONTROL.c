#include "CONTROL.h"
#include "ti_msp_dl_config.h"
#include "ENCODER.h"
#include "PidRec.h"

extern float Kp,Ki,Kd;   //直立环参数
float err;               //此次误差
float last_err;          //上次误差
float err_sum=0;         //误差累加
float err_difference;    //误差的差值

float filt_velocity;     //滤波后的速度
float last_filt_velocity;//上一次的滤波后的速度
float velocity_err_sum=0;    //速度的累加
float velocity_err;//速度的差值

int vertical_PID_value(float measure,float calcu)
{

	err = measure - calcu;             //误差
	err_sum+=err;  //误差的累加
    I_jiaoxianfu(&err_sum, 5);                    
	err_difference = err - last_err;   //误差的差值
	last_err = err;                    //此次误差记录为“上次误差”
	
	return Kp*err + Ki*err_sum + Kd*err_difference;
}

//旧速度环
// int velocity_PID_value(int velocity_measure,float velocity_calcu)
// {
// 	float a=0.3;                                               //滤波系数（反映滤波程度）
//     velocity_err = velocity_measure - velocity_calcu;
// 	filt_velocity = a*velocity_err + (1-a)*last_filt_velocity; //一阶速度滤波
// 	velocity_err_sum +=  filt_velocity;                        //速度的累加
// 	I_xianfu(&velocity_err_sum ,20000);                          //累加限幅
// 	err_difference = velocity_err - last_err;   //误差的差值
// 	last_filt_velocity = filt_velocity;                        //此次速度记录为“上次速度”
// 	last_err = velocity_err;

// 	return VKp*filt_velocity + VKi*velocity_err_sum + VKd*err_difference;
// }

//新速度环
float velocity_PID_value_new(float Encoder,float Target)
{
	static float Bias, Last_bias, Last2_bias, Pwm;
	Bias = Target - Encoder;               			                                                //计算偏差
	Pwm += VKp * (Bias - Last_bias) + VKi * Bias + VKd * (Bias - 2 * Last_bias + Last2_bias);   	//增量式PI控制器
	Last_bias = Bias;	                   															//保存上一次偏差
	Last2_bias = Last_bias;
	return Pwm;                        				                                        		//返回增量值
}


//转向环PD控制器
//输入：角速度、角度值
int Turn(float gyro_Z,int Target_turn)
{
	int temp;
    int last_turn;
    int err_turn;
    err_turn = Target_turn;
	temp=TKp*gyro_Z+TKd*(err_turn-last_turn);
    last_turn=err_turn;
	return temp;
}

//I限幅：
void I_xianfu(float *velocity_err_sum, int max)
{
	if(*velocity_err_sum>max)  *velocity_err_sum=max;
	if(*velocity_err_sum<-max) *velocity_err_sum=-max;
}

void I_jiaoxianfu(float *err_sum, int max)
{
	if(*err_sum>max)  *err_sum=max;
	if(*err_sum<-max) *err_sum=-max;
}

