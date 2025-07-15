#ifndef	__KEY_H__
#define __KEY_H__
#include "Hardware\Board\board.h"

#define KEY1    DL_GPIO_readPins(KEY_KEY_1_PORT,KEY_KEY_1_PIN)
#define KEY2    DL_GPIO_readPins(KEY_KEY_2_PORT,KEY_KEY_2_PIN)
#define KEY3    DL_GPIO_readPins(KEY_KEY_3_PORT,KEY_KEY_3_PIN)
#define Key_Disp_Time    200
#define Key_Long_Time   1000
#define KEY_NONE   0
#define KEY_PRESS   1
#define KEY_LONG   2
typedef struct{
    uint8_t key_flag;
    const uint8_t KEY_NUM;
    const uint8_t KEY_LONG_NUM;
    uint16_t key_cnt;
}Key_State;
uint8_t Key_output(void);

void Key_Init(void);
#endif

