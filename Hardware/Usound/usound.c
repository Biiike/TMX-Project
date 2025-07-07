#include "usound.h"
#include "usound.h"

uint16_t Read_usound()
{
    uint16_t distVal = DL_Timer_getCaptureCompareValue(CAPTURE_ULTRASONIC_INST, DL_TIMER_CC_0_INDEX) * 0.17;

    if(distVal > 6000)
        distVal = 0;

    return distVal;

}