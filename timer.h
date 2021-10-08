#include <hidef.h>
#include "derivative.h"

typedef enum Timer_Prescale
{
  Timer_Prescale_1 = 0,
  Timer_Prescale_2 = 1,
  Timer_Prescale_4 = 2,
  Timer_Prescale_8 = 3,
  Timer_Prescale_16 = 4,
  Timer_Prescale_32 = 5,
  Timer_Prescale_64 = 6,
  Timer_Prescale_128 = 7
} Timer_Prescale;

typedef enum Timer_PinAction
{
  Timer_Pin_Disco = 0,
  Timer_Pin_Toggle = 1,
  Timer_Pin_Clear = 2,
  Timer_Pin_Set = 3
} Timer_PinAction;

volatile void Timer_Init (
    dword busClock,  // kHz, allowing up to approx. 65.5MHz. (20MHz default)
    Timer_Prescale prescale,
    word initialOffset,  // Offset from TCNT in milliseconds
    word enableInt,  // Enables interrupts on OC0
    Timer_PinAction pinAction
) {
    // Configure PORT T[0] as output for OC0
    DDRT &= (~0b00000001);
    OC7M |= OC7M_OC7M0;

    // Timer peripheral configuration
    TSCR1 &= (~0b10000000);  // Disable the timer for programming
    TSCR1 &= (~0b01100000);  // Disable stops, freezing, and fast clear
    TSCR1 &= (~0b00011000);  // Turn off fast clear, set to legacy mode
    
    // Configure free run counting
    TSCR2 &= (~0b00001000);

    // Clear and set the prescaler
    TSCR2 &= (~0b00000111);
    TSCR2 |= (prescale & 0b00000111);

    // Configure the timer to use OC0 as output compare
    TIOS |= 0b00000001;

    // Configure the action taken on OC0 compare success
    TCTL2 &= (~0b00000011);
    TCTL2 |= (pinAction & 0b00000011);

    // Configure overflow interrupts on OC0
    TIE = enableInt ? (TIE | 0b00000001) : (TIE & (~0b00000001));

    // Set OC0's compare register to configure its period in milliseconds
    TC0 = 0xffff - busClock / (2 ^ prescale * initialOffset);

    // Enable the timer
    TSCR1 |= 0b10000000;

    return;
}
