#include "SHOW.h"
#include "stdio.h"
#include "oled_hardware_i2c.h"
uint8_t oled_buffer[16];
uint8_t encoder_buffer[16];

void SHOW_Firstpage (uint8_t Range, float pitch, float roll, float yaw)
{
    OLED_ShowString(0,2,(uint8_t *)"Pitch:",16);
    OLED_ShowString(0,4,(uint8_t *)" Roll:",16);
    OLED_ShowString(0,6,(uint8_t *)"  Yaw:",16);
    OLED_ShowString(0,0,(uint8_t *)"Range:",16);
    sprintf((char *)oled_buffer, "%6.1f", pitch);
    OLED_ShowString(7*8,2,oled_buffer,16);
    sprintf((char *)oled_buffer, "%6.1f", roll);
    OLED_ShowString(7*8,4,oled_buffer,16);
    sprintf((char *)oled_buffer, "%6.1f", yaw);
    OLED_ShowString(7*8,6,oled_buffer,16);
    sprintf((char *)oled_buffer, "%6d", Range);
    OLED_ShowString(7*8,0,oled_buffer,16);
    }

    void SHOW_Secondpage(short acc[3])
    {
    OLED_ShowString(0,2,(uint8_t *)"X:",16);
    OLED_ShowString(0,4,(uint8_t *)"Y:",16);
    OLED_ShowString(0,6,(uint8_t *)"Z:",16);
    sprintf((char *)oled_buffer, "%6.1d", acc[0]);
    OLED_ShowString(7*8,2,oled_buffer,16);
    sprintf((char *)oled_buffer, "%6.1d", acc[1]);
    OLED_ShowString(7*8,4,oled_buffer,16);
    sprintf((char *)oled_buffer, "%6.1d", acc[2]);
    OLED_ShowString(7*8,6,oled_buffer,16);    
    }

    void SHOW_Thirdpage(int get_encoder_cnt1 ,  int get_encoder_cnt2)
    {
    if(get_encoder_cnt1 >= 0)    
        OLED_ShowString(0,2,(uint8_t *)"CNT1:",16);
    else
        OLED_ShowString(0,2,(uint8_t *)"CNT1:-",16);

    if(get_encoder_cnt1>=0)
        OLED_ShowString(0,4,(uint8_t *)"CNT2:",16);
    else
        OLED_ShowString(0,4,(uint8_t *)"CNT2:",16);

    sprintf((char *)encoder_buffer, "%6.1d", get_encoder_cnt1);
    OLED_ShowString(7*8,2,encoder_buffer,16);
    sprintf((char *)encoder_buffer, "%6.1d", get_encoder_cnt2);
    OLED_ShowString(7*8,4,encoder_buffer,16);
  
    }