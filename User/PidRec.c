#include "ti_msp_dl_config.h"
#include "PidRec.h"
#include "Hardware\Board\board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

volatile float VKp = 77.0;
volatile float VKi = 3.5;
volatile float VKd = 0;

volatile float TKp = -20;
volatile float TKd = -10;

#define RX_CMD_BUFFER_SIZE 32
static char gRxCmdBuffer[RX_CMD_BUFFER_SIZE];
static volatile uint32_t gRxCmdIndex = 0;
static volatile bool gCommandReady = false;

static void parse_command(void);

/* ================= 对外接口函数实现 ================= */

void Uart0_init()
{
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

void PID_Parser_Process(void)
{
    if (gCommandReady) {
        gCommandReady = false;
        parse_command();
    }
}


static void parse_command(void)
{
    float temp_val;

    if (strncmp(gRxCmdBuffer, "TKP=", 4) == 0 || strncmp(gRxCmdBuffer, "tkp=", 4) == 0)
    {
        if (sscanf(gRxCmdBuffer, "TKP=%f", &temp_val) == 1 || sscanf(gRxCmdBuffer, "tkp=%f", &temp_val) == 1) {
            TKp = temp_val;
            lc_printf("角度 Kp 设置为 %.3f\r\n", TKp);
        } else {
            lc_printf("错误：无效的 TKP 格式\r\n");
        }
    }

    else if (strncmp(gRxCmdBuffer, "VKP=", 4) == 0 || strncmp(gRxCmdBuffer, "vkp=", 4) == 0)
    {
        if (sscanf(gRxCmdBuffer, "VKP=%f", &temp_val) == 1 || sscanf(gRxCmdBuffer, "vkp=%f", &temp_val) == 1) {
            VKp = temp_val;
            lc_printf("速度 Kp 设置为 %.3f\r\n", VKp);
        } else {
            lc_printf("错误：无效的 VKP 格式\r\n");
        }
    }

    else if (strncmp(gRxCmdBuffer, "VKI=", 4) == 0 || strncmp(gRxCmdBuffer, "vki=", 4) == 0)
    {
        if (sscanf(gRxCmdBuffer, "VKI=%f", &temp_val) == 1 || sscanf(gRxCmdBuffer, "vki=%f", &temp_val) == 1) {
            VKi = temp_val;
            lc_printf("速度 Ki 设置为 %.3f\r\n", VKi);
        } else {
            lc_printf("错误：无效的 VKI 格式\r\n");
        }
    }

    else if (strncmp(gRxCmdBuffer, "TKD=", 4) == 0 || strncmp(gRxCmdBuffer, "tkd=", 4) == 0)
    {
        if (sscanf(gRxCmdBuffer, "TKD=%f", &temp_val) == 1 || sscanf(gRxCmdBuffer, "tkd=%f", &temp_val) == 1) {
            TKd = temp_val;
            lc_printf("角度 Kd 设置为 %.3f\r\n", TKd);
        } else {
            lc_printf("错误：无效的 TKD 格式\r\n");
        }
    }

    else if (strncmp(gRxCmdBuffer, "VKD=", 4) == 0 || strncmp(gRxCmdBuffer, "vkd=", 4) == 0)
    {
        if (sscanf(gRxCmdBuffer, "VKD=%f", &temp_val) == 1 || sscanf(gRxCmdBuffer, "vkd=%f", &temp_val) == 1) {
            VKd = temp_val;
            lc_printf("速度 Kd 设置为 %.3f\r\n", VKd);
        } else {
            lc_printf("错误：无效的 VKD 格式\r\n");
        }
    }
    else
    {
        lc_printf("错误：未知指令\r\n");
    }

    // 清理缓冲区为下一次接收做准备
    memset(gRxCmdBuffer, 0, sizeof(gRxCmdBuffer));
    gRxCmdIndex = 0;
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_IIDX_RX:
            while (DL_UART_isRXFIFOEmpty(UART_0_INST) == false) {
                char receivedChar = DL_UART_receiveData(UART_0_INST);

                if (gCommandReady) {
                    break;
                }

                if (receivedChar == '\n' || receivedChar == '\r') {
                    if (gRxCmdIndex > 0) {
                        gRxCmdBuffer[gRxCmdIndex] = '\0';
                        gCommandReady = true;
                    }
                } else {
                    if (gRxCmdIndex < (RX_CMD_BUFFER_SIZE - 1)) {
                        gRxCmdBuffer[gRxCmdIndex++] = receivedChar;
                    } else {
                        gRxCmdIndex = 0;
                    }
                }
            }
            break;
        default:
            break;
    }
}