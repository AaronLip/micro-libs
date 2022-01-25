#ifndef __PWM_H__
#define __PWM_H__

typedef enum Channel {
    // Clock A
    PWM_Channel0 = 0,
    PWM_Channel1 = 1,
    PWM_Channel4 = 4,
    PWM_Channel5 = 5
    // Clock B
    PWM_Channel2 = 2,
    PWM_Channel3 = 3,
    PWM_Channel6 = 6,
    PWM_Channel7 = 7
} PWM_Channel;

typedef enum Clock {
    PWM_Clock_A,
    PWM_CLock_B,
    PWM_Clock_SA,
    PWM_ClockSB
} PWM_Clock;

typedef enum Prescale {
    PWM_Prescale1,
    PWM_Prescale2,
    PWM_Prescale4,
    PWM_Prescale8,
    PWM_Prescale16,
    PWM_Prescale32,
    PWM_Prescale64,
    PWM_Prescale128
} PWM_Prescale;

typedef byte Scale;

typedef enum Alignment {
    PWM_Align_Left,
    PWM_Align_Center
} PWM_Align;

typedef enum Polarity {
    PWM_Polarity_Low,
    PWM_Polarity_High
} PWM_Polarity;

typedef struct Clock_Config {
    PWM_Channel channel;
    PWM_Prescale prescale;
} PWM_Clock_Config;

int PWM_Init(double busClock, int interruptEnable);
int PWM_Clock_Config(PWM_Clock clock, PWM_Prescale prescale, Scale scale);
int PWM_Channel_Init(PWM_Channel channel, PWM_Align alignment, PWM_Polarity polarity, int enableConcatenate);

double PWM_GetFreq(PWM_Channel channel);
int PWM_SetFreq(PWM_Channel channel, double frequency);

double PWM_GetDuty(PWM_Channel channel);
int PWM_SetDuty(PWM_Channel channel, double duty);

#endif