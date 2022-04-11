#include <hidef.h>
#include <derivative.h>

#include <interaction/SWL_led.h>

#include "adc.h"

volatile word adc_channelReadings[8] = {0};

void adc_Init(int enableInterrupts) {
    // Pwer on the ADC peripheral and let it settle
    ATD0CTL2_ADPU = 1;

    {
        volatile word counter = 0;
        while (counter++ < 0xffff);
    }

    ATD0CTL2_AFFC = 1;  // Reading a channel clears the channel interrupt
    if (enableInterrupts) ATD0CTL2_ASCIE = 1;  // Configure interrupts

    // Set the SC bits for 8 conversions per sequence 
    ATD0CTL3 |= (8 << 3) & 0b01111000;

    // Max sample time of 2 * 2 ^ x = 16 conversion clocks for high accuracy
    ATD0CTL4_SMP = 3;

    /* We use a 20MHz bus clock, but the ADC requires <= 2MHz
     * A prescale of 2 * (x + 1) = 10 will achieve this.
     * I'd be interested in learning how to accurately infer bus rate at
     * startup */
    ATD0CTL4_PRS = 4;

    /* Results unsigned right aligned, continuously sampled begining with AN0
     * NOTE: Writing to ATD0CTL5 has the side effect of toggling conversions,
     *       so be prepared to make dummy writes to this register to restart
     *       conversion
     */
    ATD0CTL5 = 0b10110000; 
}