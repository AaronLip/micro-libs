#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include <pwm8b8.h>

double _busClock;

int PWM_Init(double busClock, int interruptEnable)
{
    _busClock = busClock;

    if (interruptEnable) {
        PWMSDN_PWMIE = 1;
    }
}