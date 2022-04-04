#include <math.h>

#include "time.h"

double Time_Convert(double timing, Time_Type source, Time_Type target) {
    
    // No conversion if the types are the same
    if (source == target) return timing;

    switch (source) {
        case Time_Interval:
            switch (target) {
                case Time_Period:
                    return timing * 2;
                case Time_Frequency:
                    return pow(2 * timing, -1);
                default:
                    return -1;
            }
            break;
        case Time_Period:
            switch (target) {
                case Time_Interval:
                    return timing / 2;
                case Time_Frequency:
                    return pow(timing, -1);
                default:
                    return -1;
            }
            break;
        case Time_Frequency:
            switch (target) {
                case Time_Interval:
                    return 2 * pow(timing, -1);
                case Time_Period:
                    return pow(timing, -1);
                default:
                    return -1;
            }
            break;
    }
}