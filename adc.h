#pragma ONCE

extern volatile word adc_channelReadings[8];

void adc_Init(int enableInterrupts);