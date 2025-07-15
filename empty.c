
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
#include "FindLine.h"
#include "Beep.h"

#define Black_Cnt1    5
#define Black_Cnt2    2
#define White_CNt1    85
#define White_CNt2    50
#define Base_Speed1    18    //基础速度
#define Base_Speed2    10    //基础速度
#define ZERO_Angle_Time     200     
uint16_t dis = 0;
int ct1,ct2;
float cnt1,cnt2;
float measure;
uint8_t key_value = 0;
float target_calcu = 0;//默认前进速度
float target_A = 0;
float target_B = 0;
float target_yaw = -2;//默认角度
float target_angle = 0;
uint8_t Find_flag = 0;
uint8_t Quan_num = 0;
int turn_value = 0;
int velocity_pwm_A = 0;
int velocity_pwm_B = 0;
uint8_t Beep_flag = 0;
uint8_t Led_flag = 0;
uint8_t Stop_flag = 0;
uint8_t mode;
uint8_t STOP;
uint8_t EN_flag = 0;
uint8_t White_flag = 0;
uint8_t flag = 0;
uint16_t Num = 0;
uint16_t Q_num = 0;
int main(void)
{
    SYSCFG_DL_init();
    SysTick_Init();
    
    Key_Init();//按键使能(先看start timer开了没)
    OLED_Init();//oled使能（未连接时需注释）
    //Uart0_init();//usb串口打印使能
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
        SHOW_Fourpage(target_angle, flag, Quan_num);
    }
}
float my_abs(float yaw)
{
    float Angle;
    if(yaw < 0) Angle = -yaw;
    else Angle = yaw;

    return Angle;
}
uint8_t Lauch_Task(void)
{
    static int Cnt = 0;
    if(mode == 1)
    {
        flag = 1;
        if(!P1 || !P2 || !P3 || !P4 || !P5 || !P6 || !P7 || !P8)
        {
            Cnt ++;
            if(Cnt > Black_Cnt1)
            {
                Cnt = 0;
                Beep_flag = 1;
                Led_flag = 1;
                EN_flag = 0;
                mode = 0;
                return 1;
            }
        }
        else {
            Cnt = 0;
        }
    }
    else if(mode == 2)
    {
        if(flag == 1)
        {
            if(!P1 || !P2 || !P3 || !P4 || !P5 || !P6 || !P7 || !P8)
            {
                Cnt ++;
                if(Cnt > Black_Cnt1)
                {
                    Cnt = 0;
                    Beep_flag = 1;
                    Led_flag = 1;
                    flag ++;
                    Quan_num++;
                }
            }
            else {
                Cnt = 0;
            }
        }
        else if(flag == 2)
        {
            if(P1 && P2 && P3 && P4 && P5 && P6 && P7 && P8)
            {
                Cnt ++;
                if(Cnt > White_CNt1)
                {
                    Quan_num ++;
                    Cnt = 0;
                    Beep_flag = 1;
                    Led_flag = 1;
                    flag --;
                    if(Quan_num == 4)
                    {
                        DL_GPIO_togglePins(LED_PORT, LED_PIN_PIN);
                        EN_flag  = 0;
                        Quan_num = 0;
                        mode = 0;
                        return 1;
                    }
                }
            }
            else{
                Cnt = 0;
            }
        }
        if(Quan_num != 0 && (Quan_num % 2 == 0))
        {
            if(wit_data.yaw <  0) target_yaw = 180 - my_abs(wit_data.yaw);
            else target_yaw = wit_data.yaw - 180;
        }
    }
    else if(mode == 3)
    {
        if(flag == 2)
        {
            if(P1 && P2 && P3 && P4 && P5 && P6 && P7 && P8)
            {
                Cnt ++;
                if(Cnt > White_CNt2)
                {
                    Quan_num ++;
                    if(Quan_num == 5)
                    {
                        EN_flag = 0;
                        Quan_num = 0;
                        mode = 0;
                        return 1;
                    }
                    Cnt = 0;
                    Beep_flag = 1;
                    Led_flag = 1;
                    flag --;
                    if(Quan_num == 1)  target_yaw = -50;
                    else if(Quan_num == 3)target_angle = 55;
                }
            }
            else{
                Cnt = 0;
            }
        }
        if((Quan_num != 0) && (Quan_num  == 3))
        {
            if(wit_data.yaw <  0) target_yaw = 180 - my_abs(wit_data.yaw);
            else target_yaw = wit_data.yaw - 180;
        }
        if(flag == 1)
        {
            Num ++;
            if(Num > ZERO_Angle_Time)
            {
                if(Quan_num == 1)
                {
                    target_yaw = 0;
                    Num = 0;
                }
            }
            if(Quan_num == 3)
            {
                Q_num ++;
                if(Q_num > ZERO_Angle_Time)
                {
                    Q_num = 0;
                    target_angle = 0;
                }
            }
            if(!P1 || !P2 || !P3 || !P4 || !P5 || !P6 || !P7 || !P8)
            {
                Cnt ++;
                if(Cnt > Black_Cnt2)
                {
                    Quan_num ++;
                    Cnt = 0;
                    Beep_flag = 1;
                    Led_flag = 1;
                    flag ++;
                }
            }
            else{
                Cnt = 0;
            }
        }
    }
    return 0;
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
        //Lauch_Task();
        encoder_update();
        ct1 = get_encoder_cnt1();
        ct2 = get_encoder_cnt2();
        encoder_Rst();
        cnt1 = (float)ct1/2;
        cnt2 = (float)ct2/2;
        measure = cnt1 + cnt2;
    }
}

void Pid_pro(void)
{
    int turn_value1 = 0,turn_value2 = 0;
    int turn_pwm = 0;
    if(flag == 1)
    {
        if(Quan_num != 0 && (Quan_num % 2 == 0) && mode == 2)turn_pwm = Turn(target_yaw, -2);
        else if(Quan_num != 0 && (Quan_num  == 3) && mode == 3)turn_pwm = Turn(target_yaw, target_angle);
        else turn_pwm = Turn(wit_data.yaw, target_yaw);
        velocity_pwm_A = (int)velocity_PID_value_new(measure, Base_Speed1);//速度环
        velocity_pwm_B = (int)velocity_PID_value_new(measure, Base_Speed1);//速度环
        set_motor_pwm(velocity_pwm_A + turn_pwm,velocity_pwm_B - turn_pwm);
    }
    else if(flag == 2)
    {
        if(mode == 3)
            turn_value2 = Find_line_task1();
        else 
            turn_value2 = Find_line_task();
        target_A = Base_Speed2 - turn_value2;
        target_B = Base_Speed2 + turn_value2;
        velocity_pwm_A = (int)velocity_PID_value_new(measure, target_A);//速度环
        velocity_pwm_B = (int)velocity_PID_value_new(measure, target_B);//速度环
        set_motor_pwm(velocity_pwm_A,velocity_pwm_B);
    }
}
void TIMER_Pid_INST_IRQHandler(void)
{
    if(DL_TimerG_getPendingInterrupt(TIMER_Pid_INST)==DL_TIMER_IIDX_ZERO) 
    {
        Stop_flag = Lauch_Task();
        if(Stop_flag || EN_flag == 0)   set_motor_pwm(0, 0);
        else if(EN_flag)                Pid_pro();
    }
}
void TIMER_Key_INST_IRQHandler(void)
{

    if(DL_TimerA_getPendingInterrupt(TIMER_Key_INST)==DL_TIMER_IIDX_ZERO) {
        key_value = Key_output();
        if(key_value == 1)
        {
            mode = 1;
            EN_flag = 1;
        }
        else if(key_value == 11)
        {
            mode = 2;
            EN_flag = 1;
            flag = 1;
        }
        else if(key_value == 2)
        {
            mode = 3;
            EN_flag = 1;
            flag = 2;
        }
    }
}
