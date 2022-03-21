#include <hidef.h>
#include "derivative.h"

#include "timer.h"
#include "runtime.h"
#include "sci.h"

#include <math.h>
#include <stdio.h>

static double busRate;

// Timer peripheral configuration
void Timer_Init(
    double busClock,            // kHz, allowing up to approx. 65.5MHz. (20MHz default)
    Timer_Prescale prescale    // Divides the bus clock signal for longer durations
) {
    // Store busRate somewhere (though we can calculate this internally if we are not using an external oscillator)
    busRate = busClock;

    TSCR1_TEN = 0;    // Disable the timer for programming
    TSCR1_TSWAI = 0;  // Allow timer to run while micro is waiting
    TSCR1_TSFRZ = 1;  // But disable while frozen for the HWAVE debugger
    TSCR1_TFFCA = 0;  // Turn off fast flag clear - clears are explicit by writing 1
    TSCR1_PRNT = 0;   // 
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

void Timer_Interrupt(Timer_Channel channel, int enable) {
    if (enable) {
        TIE |= 1 << channel; // Be cautious not to write 1s to other channels
    } else {
        TIE &= ~(1 << channel);
    }
}

#define T_OFFSET(TC_REGISTER) \
case Timer_Channel##TC_REGISTER:\
    TC##TC_REGISTER = (word) (long) Timer_Cycles(timing);\
    break;

void Timer_Channel_Init(Timer_Channel channel, Timer_Quantity timing, Timer_PinAction pinAction, int enable) {
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
    Timer_Interrupt(channel, enable);

    // Set OCx's compare register to configure the timer's period
    switch (channel) {
        T_OFFSET(0);
        T_OFFSET(1);
        T_OFFSET(2);
        T_OFFSET(3);
        T_OFFSET(4);
        T_OFFSET(5);
        T_OFFSET(6);
        T_OFFSET(7);
        default:
            break;
    }

    // Initialize channel x timer flag by clearing
    TFLG1 |= 1 << channel;  
}
#undef T_OFFSET

#define T_REARM(TC_REGISTER) \
case Timer_Channel##TC_REGISTER:\
    TC##TC_REGISTER += (word) (long) Timer_Cycles(timing);\
    break;

int Timer_Rearm(Timer_Channel channel, Timer_Quantity timing) {

    // Check the timer flag
    if (TFLG1 & (1 << channel)) {

        // Clear and Rearm the flag
        TFLG1 |= (1 << channel);
        
        // Rearm the compare count using a lookup
        switch (channel) {
            T_REARM(0);
            T_REARM(1);
            T_REARM(2);
            T_REARM(3);
            T_REARM(4);
            T_REARM(5);
            T_REARM(6);
            T_REARM(7);
            default:
                break;
        }

        // Notify the caller
        return 1;
    }   return 0;
}
#undef T_REARM

int Timer_Sleep(Timer_Quantity timing) {
    double cycles;
    dword fullDelays;
    word remainderDelay;

    // Indicate invalid configurations
    blocking_assert(timing.value >= 0, "Failed: Timer delay must be above 0\n");
    blocking_assert(TSCR1_TEN == 1, "Failed: Timer disabled\n");

    // Convert a potentially very large delay using modulo arithmetic
    cycles = Timer_Cycles(timing);
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
    blocking_assert(
        cycles >= 0 && cycles / 0xffff <= 0xffffffff,
        "Timer cycles are nonsense or exceed capacity\n"
    );

    // Return success
    return 0;
}

#define PRESCALE (word) (1 << TSCR2_PR)

// Tx / Tc yields the amount of cycles to generate a target period, where Tx = target period and Tc = period of the clock
double Timer_Cycles(Timer_Quantity timing) {
    double cycles;

    // Disable recursion warning because it is a false positive 
    #pragma MESSAGE DISABLE C1855

    switch (timing.type)
    {
        case Timer_Period:
            {
                Timer_Quantity t;
                t.type = Timer_Interval;
                t.value = timing.value / 2;

                cycles = Timer_Cycles(t); 
            }
            break;
        case Timer_Interval:
            cycles = (timing.value / 1E3) * busRate / PRESCALE;
            break;
        case Timer_Frequency:
            {
                Timer_Quantity t;
                t.type = Timer_Period;
                t.value = pow(2 * timing.value, -1.0);

                cycles = Timer_Cycles(t);
            }
            break;
        default:
            break;
    }

    // Re-enable recursion warning
    #pragma MESSAGE WARNING C1855

    #ifdef __TIMER_H_DEBUG__
    (void) blocking_assert(
        0 < cycles && cycles <= 0xffff,
        "cycles are outside of expected range\n"
    );
    #endif

    return cycles;
}

#undef PRESCALE