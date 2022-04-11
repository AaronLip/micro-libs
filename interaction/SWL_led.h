#pragma ONCE

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
void SWL_Init(void);

// Turn the specified LED on
void SWL_ON(SWL_LEDColour led);

// Turn the specified LED off
void SWL_OFF(SWL_LEDColour led);

// Toggle between on and off for the specified LED
void SWL_TOG(SWL_LEDColour led);

// Use a conditional to turn on the specified LED
void SWL_IF(int condition, SWL_LEDColour led);

// Return a truthy int (the button's mask) if the indicated button is pressed, 0 otherwise
int SWL_Pushed(SWL_SwitchPos button);

// Return a truthy int (the mask for all buttons) if any buttons are pressed, 0 otherwise
byte SWL_Any(void);

// Checks a button is pushed using a debouncing algorithm
byte SWL_Debounced(void);
byte SWL_PushedDeb(SWL_SwitchPos button);

// look for transitions (compares against previous call)
byte SWL_Transitions(void);

// look for transition by switch name (compares against previous call)
byte SWL_Transition(SWL_SwitchPos button);