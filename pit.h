#ifndef __PIT_H__
#define __PIT_H__

typedef enum Channel {
    // Clock A
    PIT_Channel0 = 0,
    PIT_Channel1 = 1,
    PIT_Channel4 = 4,
    PIT_Channel5 = 5,
    // Clock B
    PIT_Channel2 = 2,
    PIT_Channel3 = 3,
    PIT_Channel6 = 6,
    PIT_Channel7 = 7
} PIT_Channel;

typedef enum Clock {
    PWM_Clock_A = 0b0,
    PWM_CLock_B = 0b1,
    PWM_Clock_SA = 0b10,
    PWM_ClockSB = 0b11
} PWM_Clock;

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

int PIT_Init(double busClock, int interruptEnable);
int PIT_Channel_Init(PIT_Channel channel, PWM_Align alignment, PWM_Polarity polarity, int enableConcatenate);

double PIT_GetFreq(PIT_Channel channel);
int PIT_SetFreq(PIT_Channel channel, double frequency);

double PIT_GetDuty(PIT_Channel channel);
int PIT_SetDuty(PIT_Channel channel, double duty);

#endif