#pragma ONCE

typedef enum PIT_Channel {
    PIT_Channel0 = 0,
    PIT_Channel1 = 1,
    PIT_Channel2 = 2,
    PIT_Channel3 = 3
} PIT_Channel;

typedef enum PIT_Timebase {
    PIT_Timebase0 = 0,
    PIT_Timebase1 = 1
} PIT_Timebase;

typedef enum PIT_Prescale {
    PWM_Prescale1 = 0,
    PWM_Prescale2 = 1,
    PWM_Prescale4 = 2,
    PWM_Prescale8 = 4,
    PWM_Prescale16 = 5,
    PWM_Prescale32 = 6,
    PWM_Prescale64 = 7,
    PWM_Prescale128 = 8
} PWM_Prescale;

typedef enum PIT_Type {
    PIT_Interval,  // One PIT event per duration of input value
    PIT_Period,    // Two PIT events per duration of input value
    PIT_Frequency  // Twice the input value in events per second
    
} PIT_Type;

typedef struct PIT_Quantity { PIT_Type type; double value; } PIT_Quantity;

typedef struct PIT_Pair { word ld; byte mtld; } PIT_Pair;

void PIT_Timebase_Set(PIT_Timebase microTimebase, byte countdownValue);
void PIT_Channel_Set(PIT_Channel channel, word countdownValue);
void PIT_Interrupt(PIT_Channel channel, int enable);

void PIT_Init(double busClock);
void PIT_Channel_Init(
    PIT_Channel channel,
    PIT_Timebase microTimebase,
    PIT_Quantity timing,
    int enable
);

PIT_Pair PIT_Solve(PIT_Quantity timing);