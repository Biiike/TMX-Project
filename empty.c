
#include "ti_msp_dl_config.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_timer.h"
#include "board.h"
#include "key.h"
#include "usound.h"
#include "oled_hardware_i2c.h"
#include "clock.h"
#include "stdio.h"
#include "SHOW.h"
#include "mpu6050.h"
#include "wit.h"
#include "MOTOR.h"
#include "OpenMv.h"
#include "ENCODER.h" 
#include "CONTROL.h"
#include "PidRec.h"

uint16_t dis = 0;
int ct1,ct2;
float cnt1,cnt2;
float measure;

float target_calcu = 10;//默认前进速度
float target_yaw = 0;//默认角度

int turn_pwm = 0;
int velocity_pwm = 0;

int main(void)
{
    SYSCFG_DL_init();
    SysTick_Init();
    
    Key_Init();//按键使能(先看start timer开了没)
    //OLED_Init();//oled使能（未连接时需注释）
    Uart0_init();//usb串口打印使能
    //MPU6050_Init();//MPU使能,需要先配置 p10 p11 引脚
    WIT_Init();
    encoder_init();
    OpenMv_Init();

    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);


    NVIC_ClearPendingIRQ(TIMER_Pid_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_Pid_INST_INT_IRQN);
    //Set_OpmvLight(20);//opmv灯光调节。0-100

    while (1) 
    {  

    }
}


void TIMER_1_INST_IRQHandler(void)
{
    if(DL_TimerA_getPendingInterrupt(TIMER_1_INST)==DL_TIMER_IIDX_ZERO) 
    {   
        //lc_printf("pitch:%.2f, roll:%.2f, yaw:%.2f\n",wit_data.pitch,wit_data.roll,wit_data.yaw);
        //lc_printf("pitch,roll,yaw:%.2f, %.2f, %.2f\n",wit_data.pitch,wit_data.roll,wit_data.yaw);
        //lc_printf("Color:%d\n",OpenMv_Buff[1]);
        //SHOW_Firstpage (dis, wit_data.pitch, wit_data.roll, wit_data.yaw);
        //SHOW_Thirdpage(ct1,ct2);
        //lc_printf("ct1 ct2:%.d,%.d\n",ct1,ct2);
        //lc_printf("%d,%d,%1.f,%d\r\n",ct2,ct1,target_calcu,velocity_pwm);
        //PID_Parser_Process();
            
    }
}
void Pid_pro(void)
{
    encoder_update();
    ct1 = get_encoder_cnt1();
    ct2 = get_encoder_cnt2();
    encoder_Rst();
    cnt1 = (float)ct1/2;
    cnt2 = (float)ct2/2;
    measure = cnt1 + cnt2;
    turn_pwm = Turn(wit_data.yaw, target_yaw);//转向环
    velocity_pwm = (int)velocity_PID_value_new(measure, target_calcu);//速度环
    set_motor_pwm(velocity_pwm + turn_pwm,velocity_pwm - turn_pwm);
}
void TIMER_Pid_INST_IRQHandler(void)
{
    if(DL_TimerG_getPendingInterrupt(TIMER_Pid_INST)==DL_TIMER_IIDX_ZERO) 
    {
        Pid_pro();
    }
}