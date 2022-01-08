#ifndef _TIMER_H_
#define _TIMER_H_

typedef enum Timer_Prescale
{
    Timer_Prescale_1 = 0,
    Timer_Prescale_2 = 1,
    Timer_Prescale_4 = 2,
    Timer_Prescale_8 = 3,
    Timer_Prescale_16 = 4,
    Timer_Prescale_32 = 5,
    Timer_Prescale_64 = 6,
    Timer_Prescale_128 = 7
} Timer_Prescale;

typedef enum Timer_PinAction
{
    Timer_Pin_Disco = 0,
    Timer_Pin_Toggle = 1,
    Timer_Pin_Clear = 2,
    Timer_Pin_Set = 3
} Timer_PinAction;

typedef enum Timer_Channel
{
    Timer_Channel0 = 0,
    Timer_Channel1 = 1,
    Timer_Channel2 = 2,
    Timer_Channel3 = 3,
    Timer_Channel4 = 4,
    Timer_Channel5 = 5,
    Timer_Channel6 = 6,
    Timer_Channel7 = 7
} Timer_Channel;

void Timer_Init(dword busClock, Timer_Prescale prescale, word initialOffset, word enableInt, Timer_PinAction pinAction);
int Timer_Sleep(double milliseconds);
double Timer_Cycles(double delayMilliseconds);
double Timer_FreqCycles(double targetFrequency);

#endif