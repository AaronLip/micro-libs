#include <hidef.h>
#include <derivative.h>

#include "pwidth.h"

void PWM_ClockSource_Set(PWM_Channel channel, PWM_ClockSource source) {
    switch (source) {
        case PWM_ClockPrescaler:
            PWMCLK &= ~(1 << channel);
            break;
        case PWM_ClockScaler:
            PWMCLK |= 1 << channel;
            break;
        default:
            break;
    }
}

void PWM_Polarity_Set(PWM_Channel channel, PWM_Polarity polarity) {
    switch (polarity) {
        case PWM_PolarityLow:
            PWMPOL &= ~(1 << channel);
            break;
        case PWM_PolarityHigh:
            PWMPOL |= 1 << channel;
            break;
        default:
            break;
    }
}

void PWM_Clock_Init(PWM_Clock clock, PWM_Prescale prescale, PWM_Scale scale) {
    switch (clock) {
        case PWM_ClockA:
            PWMPRCLK_PCKA = prescale & 0b111;
            PWMSCLA = scale;
            break;
        case PWM_ClockB:
            PWMPRCLK_PCKB = prescale & 0b111;
            PWMSCLB = scale;
            break;
    }
}

/* The default PWM control register means PWM_Init() is not needed - allow PWM
 * to continue in wait mode and in freeze mode so debug doesn't crash
 * peripherals. */
void PWM_Channel_Init(
    PWM_Channel channel,
    PWM_Width width,
    PWM_Channel_Period period,
    PWM_Channel_Duty duty,
    PWM_ClockSource source,
    PWM_Polarity polarity
) {
    // Correct invalid duty values
    if (duty > period) duty = period;

    if (width == PWM_Width16) {
        // Concatenate the channels by setting the CON register
        PWMCTL |= (1 << (4 + channel / 2));

        channel |= 1;  // Choose the most significant channel

        // Configure the most significant bytes
        PWMPER_ARR[channel] = (period & 0xff00) >> 8;
        PWMDTY_ARR[channel] = (duty & 0xff00) >> 8;
            
        PWM_ClockSource_Set(channel, source);
        PWM_Polarity_Set(channel, polarity);

        channel &= ~1;  // Choose the least significant channel
    }

    // Configure the current 8-bit or least-significant 16-bit channel
    PWMPER_ARR[channel] = period & 0x00ff;
    PWMDTY_ARR[channel] = duty & 0x00ff;

    if (width == PWM_Width8) {
        PWM_ClockSource_Set(channel, source);
        PWM_Polarity_Set(channel, polarity);
    }

    // Switch back to most significant channel if necessary
    if (width == PWM_Width16) channel |= 1;

    // And enable the channel
    PWME |= (1 << channel);
}

void PWM_Duty(PWM_Channel channel, PWM_Channel_Duty duty) {
    PWM_Channel_Period period = 0;
    int isConcatenated = PWMCTL & (1 << (4 + channel / 2)); // Check if 16-bit PWM

    if (isConcatenated) {
        period = (PWMPER_ARR[channel | 1] << 8) | PWMPER_ARR[channel & ~1];
    } else {
        period = PWMPER_ARR[channel];
    }
    
    // Convert duty into a percentage of the period
    duty = (word) ((dword) duty * period / 0xffff);

    if (isConcatenated) {
        PWMDTY_ARR[channel | 1] = (duty & 0xff00) >> 8;
        PWMDTY_ARR[channel & ~1] = duty & 0x00ff;
    } else {
        // Because period is 8 bit and duty <= period, no data should be lost
        PWMDTY_ARR[channel] = duty & 0x00ff;
    }

    // Load new values immediately, regardless of config (i.e. infinite period)
    PWME &= ~(1 << channel);
    PWME |= (1 << channel);
}

void PWM_Colour_Init() {
    PWM_Channel_Init(PWM_Channel4, PWM_Width8, 0xff, 0, PWM_ClockScaler, PWM_PolarityHigh);
    PWM_Channel_Init(PWM_Channel1, PWM_Width8, 0xff, 0, PWM_ClockScaler, PWM_PolarityHigh);
    PWM_Channel_Init(PWM_Channel0, PWM_Width8, 0xff, 0, PWM_ClockScaler, PWM_PolarityHigh);
}

void PWM_Colour_Set(PWM_Colour colour) {
    PWM_Duty(PWM_Channel4, 0xff * colour.R);
    PWM_Duty(PWM_Channel1, 0xff * colour.G);
    PWM_Duty(PWM_Channel0, 0xff * colour.B);
}

void PWM_Backlight_Init(PWM_Channel_Duty duty) {
    PWM_Channel_Init(PWM_Channel3, PWM_Width8, 0xff, duty, PWM_ClockPrescaler, PWM_PolarityHigh);
}

void PWM_Backlight_Set(PWM_Channel_Duty duty) {
    PWM_Duty(PWM_Channel3, duty);
}

void PWM_Speaker_Init() {
    PWM_Channel_Init(PWM_Channel6, PWM_Width8, 0xff, 0, PWM_ClockPrescaler, PWM_PolarityHigh);
}