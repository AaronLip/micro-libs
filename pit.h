#ifndef __PIT_H__
#define __PIT_H__

typedef enum Channel {
    PIT_Channel0 = 0,
    PIT_Channel1 = 1,
    PIT_Channel2 = 2,
    PIT_Channel3 = 3
} PIT_Channel;

typedef enum Timebase {
    PIT_Micro_Timebase0 = 0,
    PIT_Micro_Timebase1 = 1
} PIT_Timebase;

typedef enum Prescale {
    PWM_Prescale1 = 0,
    PWM_Prescale2 = 1,
    PWM_Prescale4 = 2,
    PWM_Prescale8 = 4,
    PWM_Prescale16 = 5,
    PWM_Prescale32 = 6,
    PWM_Prescale64 = 7,
    PWM_Prescale128 = 8
} PWM_Prescale;

void PIT_Init(double busClock);
int PIT_Channel_Init(PIT_Channel channel, PIT_Timebase microTimebase, double period);

double PIT_GetFreq(PIT_Channel channel);
int PIT_SetFreq(PIT_Channel channel, double frequency);

double PIT_GetPeriod(PIT_Channel channel);
int PIT_SetPeriod(PIT_Channel channel, double period);

double PIT_GetPITDCycles(PIT_Channel channel, double period);

#endif