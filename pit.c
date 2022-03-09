#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "pit.h"

static double _busClock;

void PIT_Init(double busClock) {
    _busClock = busClock;

    /* Enable the periodic interrupt timer, configured to stall on freeze
     * mode. This allows debuggers to interact with the microcontroller. */
    PITCFLMT = 0b10100000;
}

int PIT_Channel_Init(PIT_Channel channel, PIT_Timebase microTimebase, double period) {
    byte maskedChannel = channel & 0xf;
    byte maskedTimebase = microTimebase & 0x1;
    
    // Configure the micro timebase first, in case the PIT is currently running
    PITMUX &= ~(maskedTimebase << maskedChannel);

    {
        double PITDCycles = PIT_GetPITDCycles(channel, period);

        if (0 <= PITDCycles && PITDCycles <= 0xffff) {
            switch (channel) {
                case PIT_Channel0:
                    PITLD0 = (word) PITDCycles;
                    break;
                case PIT_Channel1:
                    PITLD1 = (word) PITDCycles;
                    break;
                case PIT_Channel2:
                    PITLD2 = (word) PITDCycles;
                    break;
                case PIT_Channel3:
                    PITLD3 = (word) PITDCycles;
                    break;
                default:
                    return -1;
            }
        }
    }
    
    // Enable interrupts on the selected channel
    PITINTE |= 1 << maskedChannel;

    // Turn the channel on
    PITCE |= 1 << maskedChannel;
}

/* Get the amount of PITLD cycles required for a given period, given the
 * current configuration of a channel */
double PIT_GetPITDCycles(PIT_Channel channel, double period) {
    byte maskedChannel = channel & 0xf;
    byte timebase;

    if (PITMUX & (1 << maskedChannel)) {
        timebase = PITMTLD1;
    } else {
        timebase = PITMTLD1;
    }

    return ((timebase + 1) / period * _busClock) - 1;
}