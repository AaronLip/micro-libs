#include <hidef.h>
#include <math.h>
#include "derivative.h"
#include "timer.h"

dword busRate;

// Timer peripheral configuration
void Timer_Init(
    dword busClock,            // kHz, allowing up to approx. 65.5MHz. (20MHz default)
    Timer_Prescale prescale    // Divides the bus clock signal for longer durations
) {
    // Store busRate somewhere (though we can calculate this internally if we are not using an external oscillator)
    busRate = busClock;

    // Disable the timer for programming
    TSCR1_TEN = 0;

    TSCR1 &= (~0b01100000);  // Disable stops, freezing, and fast clear
    TSCR1 &= (~0b00011000);  // Turn off fast clear, set to legacy mode
    
    // Configure free run counting
    TSCR2 &= (~0b00001000);

    // Clear, then set the prescaler
    TSCR2_PR = 0;
    TSCR2 |= prescale & TSCR2_PR_MASK;

    // Enable the timer when config is done
    TSCR1_TEN = 1;

    return;
}

#define T_OFFSET(TC_REGISTER) TC_REGISTER = TCNT + initialOffset; break;
void Timer_Channel_Init(Timer_Channel channel, word initialOffset, Timer_PinAction pinAction, word enableInt) {
    // Use OCx as output compare
    TIOS |= 1 << channel;

    // Configure how the output pin behaves on OCx compare success
    if (channel > 3) {
        TCTL1 &= (~(0b11 << 2*(channel - 4)));
        TCTL1 |= (pinAction & 0b11) << 2*(channel - 4);
    } else {
        TCTL2 &= (~(0b11 << 2*channel));
        TCTL2 |= (pinAction & 0b11) << 2*channel;
    }

    // Configure overflow interrupts on OCx
    TIE = enableInt ? (TIE | 1 << channel) : (TIE & (~(1 << channel)));

    // Set OCx's compare register to configure the timer's period
    switch (channel) {
        case Timer_Channel0:
            T_OFFSET(TC0);
        case Timer_Channel1:
            T_OFFSET(TC1);
        case Timer_Channel2:
            T_OFFSET(TC2);
        case Timer_Channel3:
            T_OFFSET(TC3);
        case Timer_Channel4:
            T_OFFSET(TC4);
        case Timer_Channel5:
            T_OFFSET(TC5);
        case Timer_Channel6:
            T_OFFSET(TC6);
        case Timer_Channel7:
            T_OFFSET(TC7);
        default:
            break;
    }

    // Initialize channel x timer flag by clearing
    TFLG1 |= 1 << channel;  // Be cautious not to write 1s to other channels
}
#undef T_OFFSET

int Timer_Rearm(Timer_Channel channel, word cycleOffset) {

    // Check the timer flag
    if (TFLG1 & (1 << channel)) {

        // Clear and Rearm the flag
        TFLG1 |= (1 << channel);
        TFLG1 += cycleOffset;

        // Notify the caller
        return 1;
    }   return 0;
}

int Timer_Sleep(double milliseconds) {
    double cycles;
    dword fullDelays;
    word remainderDelay;

    // Return error value to indicate invalid configurations
    if (TSCR1_TEN == 0 || milliseconds <= 0) {
        return -1;
    }

    // Convert a potentially very large delay using modulo arithmetic
    cycles = Timer_Cycles(milliseconds);
    fullDelays = (dword) (long long) (cycles / 0xffff);
    remainderDelay = (word) (long) fmod(cycles, 0xffff);

    // Use OC6 to debug this function
    TIOS_IOS6 = 1;

    // Configure OC6 (pin 17) to toggle when the timer triggers
    TCTL1 &= (~0b00110000);
    TCTL1 |= (Timer_Pin_Toggle << 4);

    // Consume the delays that max out TC6
    if (fullDelays > 0) {
        for (; fullDelays-- > 0 ;) {

            // Clear the timer flag
            TFLG1 |= TFLG1_C6F_MASK;

            // Rearm the timer for its full duration (relative to TCx when rearming, not TCNT)
            TC6 += (word) 0xffff;

            // Block until the timer flag is raised when the delay elapses
            for (;;) {
                if (TFLG1_C6F) {
                    break;
                }
            }
        }
    }

    // Consume the remaining duration (if any)
    if (remainderDelay > 0 | (fullDelays > 0 && remainderDelay == 0)) {

        // Clear the timer flag
        TFLG1 |= TFLG1_C6F_MASK;

        // Rearm the timer for the small remaining duration
        TC6 += remainderDelay;

        // Block until the delay elapses
        for (;;) {
            if (TFLG1_C6F) {
                break;
            }
        }
    }

    // Clear the timer flag
    TFLG1 |= TFLG1_C6F_MASK;

    // Ensure that the values returned by Timer_Cycles make sense
    if (cycles < 0 || cycles / 0xffff > 0xffffffff) {
        return -1;
    }

    // Return success
    return 0;
}

#define PRESCALE (word) (1 << TSCR2 & TSCR2_PR_MASK)

// Tx / Tc yields the amount of cycles to generate a target period, where Tx = target period and Tc = period of the clock
double Timer_Cycles(double delayMilliseconds) {
    return (delayMilliseconds / 1E3) * busRate / PRESCALE;
}

double Timer_FreqCycles(double targetFrequency) {
    return (double) (busRate / (PRESCALE * targetFrequency)) / 2;
}

#undef PRESCALE