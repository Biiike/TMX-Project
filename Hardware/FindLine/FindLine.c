#include "FindLine.h"
int Find_line_task(void)
{
    int new_error = 0;
    if(!P1) new_error += 8;
    if(!P2) new_error += 6;
    if(!P3) new_error += 5;
    if(!P4) new_error += 3;
    if(!P5) new_error -= 3;
    if(!P6) new_error -= 5;
    if(!P7) new_error -= 6;
    if(!P8) new_error -= 8;

    return new_error;
}
