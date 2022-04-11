#pragma ONCE

/* Example Interrupt

interrupt VectorNumber_Vatd0
void adcint0() {
    int i = 0;
    for (; i < 8; i++)
        adc_channelReadings[i] = ATD0DR_ARR[i];
}

*/

extern volatile word adc_channelReadings[8];

void adc_Init(int enableInterrupts);