#ifndef __pid_H_
#define __CONTROL_H_
int vertical_PID_value(float measure,float calcu); //直立环
//int velocity_PID_value(int velocity_measure,float velocity_calcu);//旧速度环
float velocity_PID_value_new(float Encoder,float Target);
int Turn(float gyro_Z,int Target_turn);
void I_xianfu(float *velocity_err_sum, int max);                            //pwm限幅
extern float Kp,Ki,Kd;   //直立环参数
void I_jiaoxianfu(float *err_sum, int max);
void control_init(void);
#endif