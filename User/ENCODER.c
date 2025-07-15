#include "ENCODER.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

int encoder_cnt1=0;
int encoder_cnt2=0;
volatile long long pre_encoder_cnt1=0,pre_encoder_cnt2=0;
uint8_t direction=0;
volatile uint32_t gpio_flag;
volatile uint32_t gpiob_flag;



void encoder_init(void)
{
    NVIC_ClearPendingIRQ(GPIOA_INT_IRQn);
    NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

int get_encoder_cnt1(void)
{
    return encoder_cnt1;
}
int get_encoder_cnt2(void)
{
    return encoder_cnt2;
}
uint8_t get_direction(void)
{
    return direction;
}
void encoder_Rst(void)
{
    pre_encoder_cnt1=0;
    pre_encoder_cnt2=0;
}
void encoder_update(void)
{
    encoder_cnt1=pre_encoder_cnt1;
    encoder_cnt2=pre_encoder_cnt2;
}

uint32_t cnt;
uint32_t dist;

void GROUP1_IRQHandler(void)
{
    gpio_flag=DL_GPIO_getEnabledInterruptStatus(GPIOA,MOTOR_E2B_PIN|MOTOR_E2A_PIN|MOTOR_E1A_PIN|MOTOR_E1B_PIN);

    if((gpio_flag&MOTOR_E1A_PIN)==MOTOR_E1A_PIN)//A相上升沿
    {
        if(!DL_GPIO_readPins(GPIOA,MOTOR_E1B_PIN))
        {
            pre_encoder_cnt1--;
        }
        else
        {
            pre_encoder_cnt1++;
        }
    }
    else if((gpio_flag&MOTOR_E1B_PIN)==MOTOR_E1B_PIN)//B相上升沿
    {
        if(!DL_GPIO_readPins(GPIOA,MOTOR_E1A_PIN))
        {
            pre_encoder_cnt1++;
        }
        else
        {
            pre_encoder_cnt1--;
        }
    }
    if((gpio_flag&MOTOR_E2A_PIN)==MOTOR_E2A_PIN)//A2相上升沿
    {
        if(!DL_GPIO_readPins(GPIOA,MOTOR_E2B_PIN))
        {
            pre_encoder_cnt2++;
        }
        else
        {
            pre_encoder_cnt2--;
        }
    }
    else if((gpio_flag&MOTOR_E2B_PIN)==MOTOR_E2B_PIN)//B2相上升沿
    {
        if(!DL_GPIO_readPins(GPIOA,MOTOR_E2A_PIN))
        {
            pre_encoder_cnt2--;
        }
        else
        {
            pre_encoder_cnt2++;
        }
    }
    DL_GPIO_clearInterruptStatus(GPIOA,MOTOR_E2B_PIN|MOTOR_E2A_PIN|MOTOR_E1A_PIN|MOTOR_E1B_PIN);   
}