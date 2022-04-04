#pragma ONCE
#include <abstract/time.h>

typedef enum Timer_Prescale
{
    Timer_Prescale1 = 0,
    Timer_Prescale2 = 1,
    Timer_Prescale4 = 2,
    Timer_Prescale8 = 3,
    Timer_Prescale16 = 4,
    Timer_Prescale32 = 5,
    Timer_Prescale64 = 6,
    Timer_Prescale128 = 7
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

void Timer_Init(double busClock, Timer_Prescale prescale);
void Timer_Interrupt(Timer_Channel channel, int enable);
void Timer_Channel_Init(Timer_Channel channel, Time_Type type, double timing, Timer_PinAction pinAction, int enableInt);
int Timer_Rearm(Timer_Channel channel, Time_Type type, double timing);
int Timer_Sleep(Time_Type type, double timing);
double Timer_Cycles(Time_Type type, double timing);