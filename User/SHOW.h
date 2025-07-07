#ifndef _SHOW_H_
#define _SHOW_H_
#include "ti_msp_dl_config.h"
#include "oled_hardware_i2c.h"
void SHOW_Firstpage(uint8_t Range, float pitch, float roll ,float yaw);
void SHOW_Secondpage(short acc[3]);
void SHOW_Thirdpage(int get_encoder_cnt1 ,  int get_encoder_cnt2);
#endif