#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "MOTOR.h"



void ABS_pwm(int *Compare)
{
    if(*Compare<0)
        *Compare = -(*Compare);
    else 
        *Compare = *Compare;
}

void pwm_limit(int *a,int max)
{
    if(*a>max)
        *a = max;
    if(*a<-max)
        *a = -max;
}

void set_motor1_pwm(int Compare)
{
    pwm_limit(&Compare, 7000);
    ABS_pwm(&Compare);
    DL_TimerA_setCaptureCompareValue(PWM_Motor_INST, Compare, DL_TIMER_CC_1_INDEX);
}

void set_motor2_pwm(int Compare)
{
    pwm_limit(&Compare, 7000);
    ABS_pwm(&Compare);
    DL_TimerA_setCaptureCompareValue(PWM_Motor_INST, Compare, DL_TIMER_CC_0_INDEX);
}

void set_motor_pwm(int Compare1,int Compare2)
{
    if(Compare1<0)//左反
    {

        DL_GPIO_setPins(MOTOR_BIN1_L_N_PORT, MOTOR_BIN1_L_N_PIN );                   
        DL_GPIO_clearPins(MOTOR_BIN2_L_P_PORT, MOTOR_BIN2_L_P_PIN); 
    }
    else if(Compare1>0)//左正
    {
        DL_GPIO_setPins(MOTOR_BIN2_L_P_PORT, MOTOR_BIN2_L_P_PIN);                   
        DL_GPIO_clearPins(MOTOR_BIN1_L_N_PORT, MOTOR_BIN1_L_N_PIN);
    }
    else if(Compare1 == 0)
    {//停车
        DL_GPIO_clearPins(MOTOR_BIN1_L_N_PORT, MOTOR_BIN1_L_N_PIN);
        DL_GPIO_clearPins(MOTOR_BIN2_L_P_PORT, MOTOR_BIN2_L_P_PIN);
    }
    set_motor1_pwm(Compare1);

    if(Compare2<0)//傻逼商家反装轮子，这里也要反过来写
    {//右反
        DL_GPIO_clearPins(MOTOR_AIN2_R_N_PORT, MOTOR_AIN2_R_N_PIN);                   
        DL_GPIO_setPins(MOTOR_AN1_R_P_PORT, MOTOR_AN1_R_P_PIN);                  
    }
    else if(Compare2>0)
    {//右正
        DL_GPIO_clearPins(MOTOR_AN1_R_P_PORT, MOTOR_AN1_R_P_PIN);
        DL_GPIO_setPins(MOTOR_AIN2_R_N_PORT, MOTOR_AIN2_R_N_PIN);

    }
    else if(Compare2 == 0)
    {//停车
        DL_GPIO_clearPins(MOTOR_AIN2_R_N_PORT, MOTOR_AIN2_R_N_PIN);
        DL_GPIO_clearPins(MOTOR_AN1_R_P_PORT, MOTOR_AN1_R_P_PIN);
    }
    set_motor2_pwm(Compare2);
}