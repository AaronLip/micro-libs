#include <hidef.h>
#include "derivative.h"

typedef enum LEDColour {
  SWL_RED     = 0b10000000,
  SWL_YELLOW  = 0b01000000,
  SWL_GREEN   = 0b00100000
} SWL_LEDColour;

typedef enum SwitchPos {
  SWL_CTR   = 0b00000001,
  SWL_RIGHT = 0b00000010,
  SWL_DOWN  = 0b00000100,
  SWL_LEFT  = 0b00001000,
  SWL_UP    = 0b00010000 
} SWL_SwitchPos;

volatile void SWL_Init() {

    PT1AD1 &= 0b00011111;     // Configure the buffer's default values
    DDR1AD1 = 0b11100000;     // Configure I/O directions
    ATD1DIEN1 |= 0b00011111;  // Enable the buffer for switches

    return;
};

volatile void SWL_ON(SWL_LEDColour led) {

    PT1AD1 |= led;

    return;
}

volatile void SWL_OFF(SWL_LEDColour led) {

    PT1AD1 &= (~led);

    return;
}

volatile void SWL_TOG(SWL_LEDColour led) {

    PT1AD1 ^= led;

    return;
}

int SWL_Pushed(SWL_SwitchPos button) {

    return PT1AD1 & button;
}

int SWL_Any() {

    return PT1AD1 & 0b00011111;
}