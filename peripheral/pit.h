#pragma ONCE

#include <abstract/time.h>

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
    PIT_Prescale1 = 0,
    PIT_Prescale2 = 1,
    PIT_Prescale4 = 2,
    PIT_Prescale8 = 4,
    PIT_Prescale16 = 5,
    PIT_Prescale32 = 6,
    PIT_Prescale64 = 7,
    PIT_Prescale128 = 8
} PIT_Prescale;

typedef struct PIT_Pair { word ld; byte mtld; } PIT_Pair;

void PIT_Timebase_Set(PIT_Timebase microTimebase, byte countdownValue);
void PIT_Channel_Set(PIT_Channel channel, word countdownValue);
void PIT_Interrupt(PIT_Channel channel, int enable);

void PIT_Init(double busClock);
void PIT_Channel_Init(
    PIT_Channel channel,
    PIT_Timebase microTimebase,
    Time_Type type,
    double timing,
    int enable
);

PIT_Pair PIT_Solve(Time_Type type, double timing);