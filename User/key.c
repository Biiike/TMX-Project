#include "key.h"
uint8_t key_value = 0;
void Key_Init()
{
    //清除定时器中断标志
    NVIC_ClearPendingIRQ(TIMER_Key_INST_INT_IRQN);
    //使能定时器中断
    NVIC_EnableIRQ(TIMER_Key_INST_INT_IRQN);

}

 static Key_State key_state[3] = {
    {
        .key_cnt = 0,
        .key_flag = 0,
        .KEY_NUM = 1,
        .KEY_LONG_NUM = 11 //长按
    },
    {
        .key_cnt = 0,
        .key_flag = 0,
        .KEY_NUM = 2,
        .KEY_LONG_NUM = 22
    }
 };
 Key_State* key_input(void)
 {
    if(!KEY1)   return &key_state[0];
    else if(!KEY2)  return &key_state[1];

    return NULL;
}
uint8_t Key_output(void)
{
    uint8_t key_num = 0;
    Key_State* keystate = key_input();
    if(keystate == NULL)
    {
        for(uint8_t i = 0; i < 2; ++i)
        {
            if(key_state[i].key_flag == KEY_PRESS)
            {
                key_state[i].key_flag = KEY_NONE;
                key_num = key_state[i].KEY_NUM;
            }
            else if(key_state[i].key_flag == KEY_LONG)
            {
                key_state[i].key_flag = KEY_NONE;
                key_num = key_state[i].KEY_LONG_NUM;
            }
            key_state[i].key_cnt = 0;
        }
    }
    else if(!(keystate->key_flag & KEY_LONG))
    {
        keystate -> key_cnt++;
        if(keystate->key_cnt <= Key_Disp_Time)
        {
            keystate->key_flag = KEY_PRESS;
        }
        if(keystate->key_cnt >= Key_Long_Time)
        {
            keystate->key_flag = KEY_LONG;
        }
    }

    return key_num;
}

void TIMER_Key_INST_IRQHandler(void)
{

    if(DL_TimerA_getPendingInterrupt(TIMER_Key_INST)==DL_TIMER_IIDX_ZERO) {
        key_value = Key_output();
        if(key_value == 1){
            DL_GPIO_setPins(LED_PORT,LED_PIN_PIN);
        }
        else if(key_value == 11)
        {
            DL_GPIO_clearPins(LED_PORT,LED_PIN_PIN);
        }
    }
}