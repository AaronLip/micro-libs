#include <mc9s12xdp512.h>

#define ICM_WRITE PORTA &= (~0x01); PORTA |= 0x01;
#define ICM_DATA_MODE PORTA &= (~0x02);
#define ICM_CONTROL_MODE PORTA |= 0x02;

typedef enum {
    OFF,
    ON
} ICM_DECIMAL;

// Hold /WRITE low for 200+ ns when writing, then hold high for 200+ ns.
// Clocks data in on rising edges.

volatile void icm7218a_normal(byte address, byte value, ICM_DECIMAL decimal_point) {
    address &= 0x07;

    address |= 0b01011000;

    if (decimal_point)
        value &= (~0x80);
    else
        value |= 0x80;
    
    // Write control
    PORTB = address;
    ICM_CONTROL_MODE;
    ICM_WRITE;

    // Write data
    PORTB = value;
    ICM_DATA_MODE;
    ICM_WRITE;
}

volatile void icm7218a_init() {
    byte i;

    // Configure A1 and A0
    PORTA |= 0x03; // Default high
    DDRA |= 0x03;  // as outputs

    // Configure PORTB as outputs
    DDRB = 0xff;

    // Wipe display data
    for (i = 0; i < 8; i++)
        icm7218a_normal(i, 0x00, OFF);

    return;
}

volatile void control(byte control_word) {
    ICM_CONTROL_MODE;
    PORTB = control_word;
    ICM_WRITE;
}

#undef ICM_WRITE
#undef ICM_DATA_MODE
#undef ICM_CONTROL_MODE