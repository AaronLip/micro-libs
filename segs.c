#include <hidef.h>
#include "derivative.h"
#include "segs.h"
#include "timer.h"

#define Segs_WRITE PORTA &= (~0x01); PORTA |= 0x01;
#define Segs_DATA_MODE PORTA &= (~0x02);
#define Segs_CONTROL_MODE PORTA |= 0x02;

// setup ports to speak with 7-segs
void Segs_Init() {

    DDRA |= 0b00000011;   // Configure the write and mode pins as outputs
    PORTA |= 0b00000001;  // The write pin should default to high

    PORTB = 0x00;  // Default the data/address port as 0s
    DDRB = 0xff;  // Configure port B as the data/address port for output

    Segs_Clear();
}

// control segs manually
void Segs_Custom(byte Addr, byte Value) {
    byte t = 0;

    // Trim the address
    // Configure for bank A, normal operation, no decode (hex display), control w/o data
    PORTB = Addr & 0x07 | 0b01111000;
    Segs_CONTROL_MODE;
    Segs_WRITE;

    for (;t<8;t++);
    
    // Write the value
    PORTB = Value;
    Segs_DATA_MODE;
    Segs_WRITE;

    for (t=0;t<8;t++);
}

// show HEX decoding (changes all of display to HEX, w/wo dp)
void Segs_Normal(byte Addr, byte Value, Segs_DPOption dp) {
    byte t = 0;

    // Trim the address
    // Configure for bank A, normal operation, decode hex, control w/o data
    PORTB = Addr & 0x07 | 0b01011000;
    Segs_CONTROL_MODE;
    Segs_WRITE;

    for (;t<8;t++);

    // Clip the value into range and pack the decimal value with it
    Value &= 0xf;
    if (dp == Segs_DP_ON)
        Value &= (~0x80);
    else
        Value |= 0x80;
    
    // Write the value
    PORTB = Value;
    Segs_DATA_MODE;
    Segs_WRITE;

    for (t=0;t<8;t++);
}

// deprecated after term 1202
// go code B (changes all of display to code b)
void Segs_CodeB(byte Addr, byte Value);

// show a 16-bit value on the upper or lower display
void Segs_16H(word Value, Segs_LineOption Line) {
    byte addr = 0;

    for (;addr < 4;addr++) {

        Segs_Normal(
            // Trim and adjust the address for the current line
            (3 - addr + 4 * Line) & 0x07 | 0b01011000,
            // Trim the desired nibble
            Value / (0x1 << addr * 4) % 0x10,
            // output without a decimal point
            Segs_DP_OFF);
    }
}

// show a 16-bit value on the upper or lower display with specified decimal points
void Segs_16HP(word Value, Segs_LineOption Line, Segs_DPOption Decimals[4]) {
    byte addr = 0;

    for (;addr < 4;addr++) {

        Segs_Normal(
            // Trim and adjust the address for the current line
            (3 - addr + 4 * Line) & 0x07 | 0b01011000,
            // Trim the desired nibble
            Value / (0x1 << addr * 4) % 0x10,
            // output decimal setting
            Decimals[3-addr]);
    }
}

// show a decimal value on the first or second line of the 7-segs
void Segs_16D(word Value, Segs_LineOption Line) {
    byte addr = 0;
    word magnitude = 1;

    for (; addr < 4; addr++, magnitude *= 10) {
        Segs_Normal(
            // Trim and adjust the address for the current line
            (3 - addr + 4 * Line) & 0x07 | 0b01011000,
            // Trim the desired nibble
            Value / magnitude % 10,
            // output without a decimal point
            Segs_DP_OFF);
    }
}

// show a 16-bit value on the upper or lower display with specified decimal points
void Segs_16DP(word Value, Segs_LineOption Line, Segs_DPOption Decimals[4]) {
    byte addr = 0;
    word magnitude = 1;

    for (; addr < 4; addr++, magnitude *= 10) {
        Segs_Normal(
            // Trim and adjust the address for the current line
            (3 + 4 * Line - addr) & 0x07 | 0b01011000,
            // Trim the desired nibble
            Value / magnitude,
            // output decimal setting
            Decimals[3-addr]);
    }
}

// show the 8-bit value starting on the digit as addr (0-6)
void Segs_8H(byte Addr, byte Value) {
    Segs_Normal(Addr % 8, (byte)((Value & 0xf0) >> 4), Segs_DP_OFF);
    Segs_Normal((Addr + 1) % 8, Value & 0xf, Segs_DP_OFF);
}

// clear the display
void Segs_Clear() {
    byte addr = 0;

    for (;addr < 8;addr++) {
        Segs_Custom(addr, 0b10000000);
    }
}

// say Err on the appropriate line
void Segs_SayErr(Segs_LineOption Line) {
    byte addr = 4 * Line;

    Segs_Custom(addr, 0b11001111);    // E
    Segs_Custom(addr+1, 0b10001100);  // r
    Segs_Custom(addr+2, 0b00001100);  // r.
    Segs_Custom(addr+3, 0b10000000);  // <Blank>
}

// deprecated after term 1202
// say 'help' as best the 7-segs can show
void Segs_SayHelp(void);

#undef Segs_WRITE
#undef Segs_DATA_MODE
#undef Segs_CONTROL_MODE