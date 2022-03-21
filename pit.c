#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include <math.h>
#include <stdio.h>

#include "pit.h"
#include "runtime.h"
#include "sci.h"

static double _busClock;

void PIT_Timebase_Set(PIT_Timebase microTimebase, byte countdownValue) {
    switch (microTimebase)
    {
        case PIT_Timebase0:
            PITMTLD0 = countdownValue;
            break;
        case PIT_Timebase1:
            PITMTLD1 = countdownValue;
            break;
        default:
            break;
    }
}

void PIT_Channel_Set(PIT_Channel channel, word countdownValue) {
    switch (channel) {
        case PIT_Channel0:
            PITLD0 = countdownValue;
            break;
        case PIT_Channel1:
            PITLD1 = countdownValue;
            break;
        case PIT_Channel2:
            PITLD2 = countdownValue;
            break;
        case PIT_Channel3:
            PITLD3 = countdownValue;
            break;
        default:
            break;
    }
}

// Enable interrupts on the selected channel
void PIT_Interrupt(PIT_Channel channel, int enable) {
    
    if (enable) {
        PITINTE |= 1 << channel;
    } else {
        PITINTE &= ~(1 << channel);
    }
    
}

void PIT_Init(double busClock) {
    _busClock = busClock;

    /* Enable the periodic interrupt timer, configured to stall on freeze
     * mode. This allows debuggers to interact with the microcontroller. */
    PITCFLMT = 0b10100000;
}

void PIT_Channel_Init(PIT_Channel channel, PIT_Timebase microTimebase, PIT_Quantity timing, int enable) {
    // Configure the micro timebase first, in case the PIT is currently running
    PITMUX &= ~(microTimebase << channel);

    {
        PIT_Pair solution = PIT_Solve(timing);

        PIT_Timebase_Set(microTimebase, solution.mtld);
        PIT_Channel_Set(channel, solution.ld);
    }
    
    // Enable interrupts on the selected channel
    PIT_Interrupt(channel, enable);

    // Turn the channel on
    PITCE |= 1 << channel;
}

PIT_Pair PIT_Solve(PIT_Quantity timing) {
    double cycles = (10 * timing.value * _busClock / 1e6 + 5) / 10;

    blocking_assert(
        0 < cycles && cycles <= ((dword) 0xffff + 1) * (0xff + 1),
        "Failed: Desired PIT duration is not possible\n"
    );

    {
        PIT_Pair pair = { 0, 1 };

        // Disable unnecessary info message about conditions being always true/false
        #pragma MESSAGE DISABLE C4000
        #pragma MESSAGE DISABLE C4001

        while ((word) pair.mtld + 1 <= 0xff && (
            (byte) (int) fmod(cycles, pair.mtld) >= 1 \
            || cycles / pair.mtld > 0xffff
        )) {
            ++pair.mtld;
        }

        blocking_assert(pair.mtld <= 0xff, "Failed: No factors could be found");

        pair.ld = (word) (long) (cycles / pair.mtld - 1);
        pair.mtld -= 1;

        return pair;
    }
}