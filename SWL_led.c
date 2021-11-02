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
#include "derivative.h"
#include "SWL_led.h"

// Calling this function puts the adc into a known state the library expects
void SWL_Init() {

    PT1AD1 &= 0b00011111;     // Set LEDS off on init
    DDR1AD1 = 0b11100000;     // Configure leds as outputs and switches as inputs
    ATD1DIEN1 |= 0b00011111;  // Use switches as digital inputs (HIGH or LOW only)

    return;
};

// Turns on an LED
void SWL_ON(SWL_LEDColour led) {

    PT1AD1 |= led;

    return;
}

// Turns off an LED
void SWL_OFF(SWL_LEDColour led) {

    PT1AD1 &= (~led);

    return;
}

// Toggles an LED
void SWL_TOG(SWL_LEDColour led) {

    PT1AD1 ^= led;

    return;
}

// Checks if a button is being pressed
int SWL_Pushed(SWL_SwitchPos button) {

    return PT1AD1 & button;
}

// Checks if any button is being pressed
// returns a truthy integer that can be used to determine which buttons are on
int SWL_Any() {

    return PT1AD1 & 0b00011111;
}