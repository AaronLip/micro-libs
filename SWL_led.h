#ifndef _SWL_led_H_
#define _SWL_led_H_

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

// Call this function to prepare the adc for this library's use
extern void SWL_Init(void);

// Turn the specified LED on
extern void SWL_ON(SWL_LEDColour led);

// Turn the specified LED off
extern void SWL_OFF(SWL_LEDColour led);

// Toggle between on and off for the specified LED
extern void SWL_TOG(SWL_LEDColour led);

// Return a truthy int (the button's mask) if the indicated button is pressed, 0 otherwise
extern int SWL_Pushed(SWL_SwitchPos button);

// Return a truthy int (the mask for all buttons) if any buttons are pressed, 0 otherwise
extern int SWL_Any(void);

#endif