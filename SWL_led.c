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
#include "timer.h"

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

// Turn on LED if condition is true
void SWL_IF(int condition, SWL_LEDColour led) {
    if (condition) SWL_ON(led);
    else SWL_OFF(led);
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

// Checks a button is pushed using a debouncing algorithm
int SWL_PushedDeb(SWL_SwitchPos pos) {
    int last = SWL_Pushed(pos);

    {
        byte i = 0;

        for (; i < 16; i++) {
            (void) Timer_Sleep(1);
            if (SWL_Pushed(pos) != last) {
                return 0;
            }
        }

        return last;
    }
}

// look for transitions (compares against previous call)
int SWL_Transitions(DebounceOption deb) {
    static int last;
    int current = SWL_Any();

    // Allow some time for bouncing to settle the current sample
    if (deb == SWL_DebOn) {
        byte i = 0;

        for (; i < 16; i++) {
            (void) Timer_Sleep(1);
            if (current != SWL_Any()) {
                return 0;
            }
        }
    }

    // Return any transitions between the current and last sample
    if (current != last) {
        int tmp = last;
        last = current;
        return current ^ tmp;
    } else {
        last = current;
        return 0;
    }
}

// look for transition by switch name (compares against previous call)
int SWL_Transition(SWL_SwitchPos pos, DebounceOption deb) {
    return SWL_Transitions(deb) & pos;
}