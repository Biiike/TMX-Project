
#include "ti_msp_dl_config.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_timer.h"
#include "board.h"
#include "bsp_gyro.h"
#include "key.h"
#include "usound.h"
#include "oled_hardware_i2c.h"
#include "clock.h"
#include "stdio.h"
#include "SHOW.h"
#include "mpu6050.h"
#include "wit.h"

uint16_t dis = 0;
int main(void)
{
    SYSCFG_DL_init();
    //jy61pInit();
    SysTick_Init();
    Key_Init();//按键使能(先看start timer开了没)
    //OLED_Init();//oled使能（未连接时需注释）
    Uart0_init();//usb串口打印使能
    //MPU6050_Init();//MPU使能
    WIT_Init();
    
    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);

    while (1) 
    {  
        //lc_printf("pitch:%.2f, roll:%.2f, yaw:%.2f",wit_data.pitch,wit_data.roll,wit_data.yaw);
    }
}


void TIMER_1_INST_IRQHandler(void)
{
    if(DL_TimerA_getPendingInterrupt(TIMER_1_INST)==DL_TIMER_IIDX_ZERO) 
    {   
        lc_printf("pitch:%.2f, roll:%.2f, yaw:%.2f\n",wit_data.pitch,wit_data.roll,wit_data.yaw);

    }
}