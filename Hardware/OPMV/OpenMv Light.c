#include "ti_msp_dl_config.h" 
#include "OpenMv Light.h"
uint8_t current_duty = 0;

void send_duty_cycle_command(UART_Regs *uart_instance, uint8_t duty_cycle) {

    if (duty_cycle > 100) {
        duty_cycle = 100; 
    }

    uint8_t packet[4];
    packet[0] = LIGHT_PACKET_HEADER;                     
    packet[1] = duty_cycle;                        
    packet[2] = (packet[0] + packet[1]) & 0xFF;    
    packet[3] = LIGHT_PACKET_FOOTER;                     


    for (int i = 0; i < 4; i++) {
        // 等待TX FIFO有空间
        while (DL_UART_isTXFIFOFull(uart_instance));
        // 写入一个字节到TX FIFO
        DL_UART_transmitData(uart_instance, packet[i]);
    }
}

void Set_OpmvLight(unsigned char duty)
{
        current_duty = duty;
        send_duty_cycle_command(UART_OPMV_INST,current_duty);
}
