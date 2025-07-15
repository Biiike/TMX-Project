#ifndef _FINDLINE_H_
#define _FINDLINE_H_

#include "ti_msp_dl_config.h"

#define P1  DL_GPIO_readPins(FIND_Left_X4_PORT,FIND_Left_X4_PIN)
#define P2  DL_GPIO_readPins(FIND_Left_X3_PORT,FIND_Left_X3_PIN)
#define P3  DL_GPIO_readPins(FIND_Left_X2_PORT,FIND_Left_X2_PIN)
#define P4  DL_GPIO_readPins(FIND_Left_X1_PORT,FIND_Left_X1_PIN)
#define P5  DL_GPIO_readPins(FIND_Right_X1_PORT,FIND_Right_X1_PIN)
#define P6  DL_GPIO_readPins(FIND_Right_X2_PORT,FIND_Right_X2_PIN)
#define P7  DL_GPIO_readPins(FIND_Right_X3_PORT,FIND_Right_X3_PIN)
#define P8  DL_GPIO_readPins(FIND_Right_X4_PORT,FIND_Right_X4_PIN)
int Find_line_task(void);
#endif