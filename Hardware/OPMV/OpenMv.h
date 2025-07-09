#ifndef	__OPENMV_H__
#define __OPENMV_H__
#include "board.h"
#define USART_RECEIVE_LENGTH    6
#define PACKET_HEADER   0xAA
#define PACKET_FOOTER   0x55
typedef struct {
    uint8_t Color_mode; //颜色
    uint8_t cx; //坐标x
    uint8_t cy; //坐标y
    uint8_t check_num;  //校验位
}Color_Packet;
extern char OpenMv_Buff[];

void OpenMv_Init(void);
void Uart_task(void);
#endif

