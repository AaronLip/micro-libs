#pragma ONCE

typedef enum Time_Type {
    Time_Interval,  // One timer event per duration of input value
    Time_Period,    // Two timer events per duration of input value
    Time_Frequency  // Twice the input value in events per second
} Time_Type;

double Time_Convert(double timing, Time_Type source, Time_Type target);