#include <hidef.h>
#include <math.h>
#include "derivative.h"
#include "timer.h"

dword busRate;

// Timer peripheral configuration
void Timer_Init (
    dword busClock,            // kHz, allowing up to approx. 65.5MHz. (20MHz default)
    Timer_Prescale prescale,   // Divides the bus clock signal for longer durations
    word initialOffset,        // Offset from TCNT in milliseconds
    word enableInt,            // Enables interrupts on OC0
    Timer_PinAction pinAction  // Pin 9's behaviour when the timer triggers
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

    // preconfigure channel 0 for manual use
    if (initialOffset != 0) {
        // Use OC0 as output compare
        TIOS_IOS0 = 1;

        // Configure how pin 9 behaves on OC0 compare success
        TCTL2 &= (~0b00000011);
        TCTL2 |= (pinAction & 0b00000011);

        // Configure overflow interrupts on OC0
        TIE = enableInt ? (TIE | TIE_C0I_MASK) : (TIE & (~TIE_C0I_MASK));

        // Set OC0's compare register to configure the timer's period
        TC0 = TCNT + initialOffset;

        // Initialize channel 0 timer flag by clearing
        TFLG1 |= TFLG1_C0F_MASK;  // Be cautious not to write 1s to other channels
    }

    // Enable the timer when config is done
    TSCR1_TEN = 1;

    return;
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

    // Ensure that the values returned by Timer_Cycles make sense
    if (cycles == 0 || cycles / 0xffff > 0xffffffff) {
        return -1;
    }

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
    if (remainderDelay > 0) {

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