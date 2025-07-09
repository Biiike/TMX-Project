#include "OpenMv.h"
#include "board.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_uart_main.h"

char OpenMv_Buff[USART_RECEIVE_LENGTH];
volatile int OpenMv_Receive_Length = 0;
volatile bool Receive_Flag = false;

void OpenMv_Init(void)
{
    NVIC_ClearPendingIRQ(UART_OPMV_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_OPMV_INST_INT_IRQN);

}

bool Check_num_task(Color_Packet* pack)
{
    uint8_t sum = (pack->Color_mode+pack->cx+pack->cy)&0xff;
    return (sum == pack->check_num);
}
void Uart_task(void)
{
    if(Receive_Flag)
    {
        Receive_Flag = false;
        __disable_irq();
        Color_Packet pack ={
            .Color_mode = OpenMv_Buff[1],
            .cx = OpenMv_Buff[2],
            .cy = OpenMv_Buff[3],
            .check_num = OpenMv_Buff[4]
        };
        __enable_irq();
        if(Check_num_task(&pack))
        {
            switch(pack.Color_mode)
            {
                case 1:
                    DL_GPIO_setPins(LED_PORT,LED_PIN_PIN);
                    break;
                case 2:
                    DL_GPIO_clearPins(LED_PORT,LED_PIN_PIN);
                    break;
                case 3:
                    DL_GPIO_togglePins(LED_PORT, LED_PIN_PIN);
                    break;
                default:
                    break;
            }
        }
    }
}
void UART_OPMV_INST_IRQHandler()
{
    if(DL_UART_getPendingInterrupt(UART_OPMV_INST) == DL_UART_IIDX_RX)
    {
        uint8_t Data = DL_UART_Main_receiveData(UART_OPMV_INST);
        if(OpenMv_Receive_Length==0 && Data != PACKET_HEADER)
        {
            return;
        }
        OpenMv_Buff[OpenMv_Receive_Length++] = Data;

        if(OpenMv_Receive_Length == USART_RECEIVE_LENGTH)
        {
           if( OpenMv_Buff[USART_RECEIVE_LENGTH-1] == PACKET_FOOTER)
           {
                Receive_Flag = true;
           }
           OpenMv_Receive_Length = 0;
        }
        Uart_task();
        
    }
    
}

