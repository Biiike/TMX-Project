/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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

uint16_t dis = 0;
int main(void)
{
    SYSCFG_DL_init();
    //jy61pInit();
    SysTick_Init();
    //Key_Init();//按键使能(先看start timer开了没)
    OLED_Init();//oled使能（未连接时需注释）
    Uart0_init();//usb串口打印使能
    MPU6050_Init();//MPU使能
    
    NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);

    while (1) 
    {  

    }
}


void TIMER_1_INST_IRQHandler(void)
{
    if(DL_TimerA_getPendingInterrupt(TIMER_1_INST)==DL_TIMER_IIDX_ZERO) 
    {   
        SHOW_Firstpage (dis, pitch, roll, yaw);
    }

}