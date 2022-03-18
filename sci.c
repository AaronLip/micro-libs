#include <hidef.h>
#include "derivative.h"

#include "runtime.h"

int SCI0_Read(char *data) {
    if (SCI0SR1_RDRF) {
        *data = SCI0DRL;
        return 1;
    } return 0;
}

byte SCI0_BlockRead() {
    while (!SCI0SR1_RDRF);

    return SCI0DRL;
}

void SCI0_BlockWrite(byte data) {
    while (!SCI0SR1_TDRE);

    SCI0DRL = data;
}

void SCI0_WriteString(char const *stringAddr) {
    for (; *stringAddr; stringAddr++) {
        SCI0_BlockWrite(*stringAddr);
    }
}

double SCI0_Init(double busClock, double baudRate) {
    // Clear and set the baud rate, using a x10+5 modifier to correctly round
    SCI0BD_SBR = (word) (long) ((10 * busClock / (16 * baudRate)) + 5) / 10;

    // Enable transmit and receive
    SCI0CR2_TE = 1;
    SCI0CR2_RE = 1;

    // Issue an ANSI clear and cursor reset to connected listeners
    SCI0_WriteString("\033[2J\033[H");

    return busClock / (16 * SCI0BD_SBR);
}

int SCI0_ReadString(char *buffer, int bufferLength) {
    blocking_assert(
        bufferLength > 0,
        "Assertion Failure: buffer length must be greater than 0\n"
    );
    
    {
        int length;

        for (length = 0; length < bufferLength - 1; length++) {
            byte data = SCI0_BlockRead();
            SCI0_BlockWrite(data);

            switch (data) {
                case '\r':
                    *buffer = '\0';
                    return length;
                default:
                    *buffer = data;
                    buffer++;
                    break;
            }
        }
        *buffer = '\0';
        return length;
    }
}