#include <hidef.h>
#include <derivative.h>

#include "portj.h"

void portj_Init() {

    // Configure port J buttons to cause interrupts
    PTJ &= 0b11111100;
    DDRJ &= 0b11111100;
    PPSJ |= 0b00000011;
    PIEJ |= 0b00000011;
}