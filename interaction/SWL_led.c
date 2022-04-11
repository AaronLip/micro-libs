/**
 * Library: Switches & LEDs
 * Author: Aaron Lip
 * Date: October 7th 2021
 * Details:
 *      Provides a simple and CMPE1250 conformant API to the user interfaces
 *      of CMPE1250's microcontroller dev board. Supports 3 LEDs and 5 switches
 * 
 * Revisions:
 *      - Tracked by local git repo
 */

#include <hidef.h>
#include <derivative.h>

#include <peripheral/timer.h>

#include "SWL_led.h"

void SWL_Init() {

    PT1AD1 &= 0b00011111;     // Set LEDS off on init
    DDR1AD1 = 0b11100000;     // Configure leds as outputs and switches as inputs
    ATD1DIEN1 |= 0b00011111;  // Use switches as digital inputs (HIGH or LOW only)

    return;
};

void SWL_ON(SWL_LEDColour led) {

    PT1AD1 |= led;

    return;
}

void SWL_OFF(SWL_LEDColour led) {

    PT1AD1 &= (~led);

    return;
}

void SWL_TOG(SWL_LEDColour led) {

    PT1AD1 ^= led;

    return;
}

void SWL_IF(int condition, SWL_LEDColour led) {
    if (condition) SWL_ON(led);
    else SWL_OFF(led);
}

int SWL_Pushed(SWL_SwitchPos button) {

    return PT1AD1 & button;
}

byte SWL_Any() {

    return PT1AD1 & 0b00011111;
}

byte SWL_Debounced() {
    static byte samples[16];
    static int index = 0;

    // get a new sample and update index
    samples[index++] = SWL_Any();
    index %= 16;

    // Sum the samples, assuming that OPEN readings are bouncy
    {
        byte i = 0, accumulator = 0xff;
        for (; i < 16; i++) {
            accumulator &= samples[i];
        }

        return accumulator;
    }
}

byte SWL_PushedDeb(SWL_SwitchPos button) {
    return SWL_Debounced() & button;
}

byte SWL_Transitions() {
    static byte last = 0;
    byte temp = last, current = SWL_Debounced();

    last = current;

    return temp ^ current;
}

byte SWL_Transition(SWL_SwitchPos button) {
    return SWL_Transitions() & button;
}

byte SWL_Click(SWL_SwitchPos button) {
    return button & SWL_Transition(button) & SWL_Any();
}

byte SWL_Release(SWL_SwitchPos button) {
    return button & SWL_Transition(button) & !(SWL_Any() & button);
}