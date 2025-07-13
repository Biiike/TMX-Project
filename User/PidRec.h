

#ifndef PIDREC_H_
#define PIDREC_H_

#include <stdbool.h>

extern volatile float VKp;
extern volatile float VKi;
extern volatile float VKd;

extern volatile float TKp;
extern volatile float TKd;


void Uart0_init();

void PID_Parser_Process(void);


#endif 