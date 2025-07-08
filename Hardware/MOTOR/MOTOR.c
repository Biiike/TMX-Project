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
    DL_TimerG_setCaptureCompareValue(PWM_Motor_INST, Compare, DL_TIMER_CC_0_INDEX);
}

void set_motor2_pwm(int Compare)
{
    pwm_limit(&Compare, 7000);
    ABS_pwm(&Compare);
    DL_TimerG_setCaptureCompareValue(PWM_Motor_INST, Compare, DL_TIMER_CC_1_INDEX);
}

void set_motor_pwm(int Compare1,int Compare2)
{
    if(Compare1>0)//左
    {//左正转
        DL_GPIO_setPins(GPIOA, MOTOR_BIN2_L_P_PIN);                   
        DL_GPIO_clearPins(GPIOA, MOTOR_BIN1_L_N_PIN);
    }
    else if(Compare1<0)
    {//左反转
        DL_GPIO_clearPins(GPIOA, MOTOR_BIN1_L_N_PIN );                   
        DL_GPIO_setPins(GPIOA, MOTOR_BIN2_L_P_PIN);
    }
    else
    {//停车
        DL_GPIO_clearPins(GPIOA, MOTOR_BIN1_L_N_PIN);
        DL_GPIO_clearPins(GPIOA, MOTOR_BIN2_L_P_PIN);
    }
    set_motor1_pwm(Compare1);

    if(Compare2>0)//右
    {//右正转
        DL_GPIO_setPins(GPIOA, MOTOR_AN1_R_P_PIN);
        DL_GPIO_clearPins(GPIOA, MOTOR_AIN2_R_N_PIN);                   
    }
    else if(Compare2<0)
    {//右反转
        DL_GPIO_clearPins(GPIOA, MOTOR_AIN2_R_N_PIN);                   
        DL_GPIO_setPins(GPIOA, MOTOR_AN1_R_P_PIN);
    }
    else
    {//停车
        DL_GPIO_clearPins(GPIOB, MOTOR_AIN2_R_N_PIN);
        DL_GPIO_clearPins(GPIOB, MOTOR_AN1_R_P_PIN);
    }
    set_motor2_pwm(Compare2);
}