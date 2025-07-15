#include "FindLine.h"
int Find_line_task(void)
{
    int new_error = 0;
    if(!P1) new_error += 9;
    if(!P2) new_error += 8;
    if(!P3) new_error += 4;
    if(!P4) new_error += 0;
    if(!P5) new_error -= 0;
    if(!P6) new_error -= 4;
    if(!P7) new_error -= 8;
    if(!P8) new_error -= 9;

    return new_error;
}
int Find_line_task1(void)
{
    int new_error = 0;
    //if(!P1) new_error += 8;
    if(!P2) new_error += 9;
    if(!P3) new_error += 7;
    if(!P4) new_error += 4;
    if(!P5) new_error -= 4;
    if(!P6) new_error -= 7;
    if(!P7) new_error -= 9;
    //if(!P8) new_error -= 8;

    return new_error;
}
