#include "ti_msp_dl_config.h"
#include "Beep.h"

void Beep_on(void)
{
    DL_GPIO_setPins(Beep_PORT, Beep_Beep_Pin_PIN);
}
void Beep_off(void)
{
    DL_GPIO_clearPins(Beep_PORT, Beep_Beep_Pin_PIN);
}