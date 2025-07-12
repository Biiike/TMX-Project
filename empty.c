
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
uint16_t dis = 0;
int ct1,ct2;
uint8_t oled_buffer[32];
int main(void)
{
    SYSCFG_DL_init();
    SysTick_Init();
    //VL53L0X_Init();
    
    Key_Init();//按键使能(先看start timer开了没)
    OLED_Init();//oled使能（未连接时需注释）
    Uart0_init();//usb串口打印使能
    //MPU6050_Init();//MPU使能,需要先配置 p10 p11 引脚
    //WIT_Init();
    encoder_init();
    OpenMv_Init();

    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);

    //Set_OpmvLight(20);//opmv灯光调节。0-100

    while (1) 
    {  
        //set_motor_pwm(-1000,-1000);//左 右
    }
}


void TIMER_1_INST_IRQHandler(void)
{
    if(DL_TimerA_getPendingInterrupt(TIMER_1_INST)==DL_TIMER_IIDX_ZERO) 
    {   
        //lc_printf("pitch:%.2f, roll:%.2f, yaw:%.2f\n",wit_data.pitch,wit_data.roll,wit_data.yaw);
        //lc_printf("Color:%d\n",OpenMv_Buff[1]);
        //SHOW_Firstpage (dis, wit_data.pitch, wit_data.roll, wit_data.yaw);

        encoder_update();
        ct1 = get_encoder_cnt1();
        ct2 = get_encoder_cnt2();
        encoder_Rst();
        SHOW_Thirdpage(ct1,ct2);
        lc_printf("ct1:%.d, ct2:%.d\n",ct1,ct2);

    }
}
