#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "lcd.h"
#include "timer.h"

// Toggle bus direction
#define lcd_ReadMode DDRH = 0; PORTK |= 2;
#define lcd_WriteMode PORTK &= (~2); DDRH = 0xFF;

// Signal to the lcd MPU when the bus is ready
#define lcd_StartTransfer PORTK |= 1;
#define lcd_StopTransfer PORTK &= (~1);

// Toggle between instruction/flags, or data
#define lcd_DataMode PORTK |= 4;
#define lcd_InstructionMode PORTK &= (~4);

// Initializes the LCD
void lcd_Init() {

    // Turn the bus off entirely while the lcd might be booting up
    lcd_StopTransfer;
    lcd_WriteMode;
    lcd_InstructionMode;

    // Configure lcd control pins as output on K
    PORTK &= (~0x07);
    DDRK |= 0x07; 

    // Wait for display MPU to boot itself
    Timer_Init(20E6, Timer_Prescale_1, 0, 0, Timer_Pin_Disco);
    (void) Timer_Sleep(15);

    // Begin 8-bit init handshake; show the 8-bit for 4.1ms, then 100us, then a final time
    PTH = 0x30;  // Function set 8-bit (bits 0:3 don't matter)

    // Long knock
    lcd_StartTransfer;
    (void) Timer_Sleep(4.1);
    lcd_StopTransfer;

    // Short knock
    lcd_StartTransfer;
    (void) Timer_Sleep(0.0001);
    lcd_StopTransfer;

    // Final knock -- bit banging is done, lcd_Await() will now work
    lcd_StartTransfer;
    (void) Timer_Sleep(0.450);
    lcd_StopTransfer;

    // Initial configuration
    lcd_Inst(0x38);  // 2-line display, 5x7 font
    lcd_DisplayControl(0, 0, 0);  // Display off
    lcd_Clear();
    lcd_Inst(0x06);  // Move cursor on write, don't shift
    lcd_DisplayControl(1, 1, 1);  // Display on, cursor on, blink cursor
    lcd_Addr(0);
}

// Writes an instruction to the LCD bus
void lcd_Inst(byte instruction) {
    // Wait for the MCU to finish tasks
    lcd_Await();

    // Prepare the bus to submit an instruction
    lcd_WriteMode;
    lcd_InstructionMode;
    PTH = instruction;

    // Set up time (tAS) after RS/RW changes is 140ns minimum
    (void) Timer_Sleep(0.140);

    lcd_StartTransfer;
    (void) Timer_Sleep(0.450);
    lcd_StopTransfer;

    // the data bus and controls must be unchanged for 10ns after a transfer ends
    (void) Timer_Sleep(0.010);
    PTH ^= PTH;

    // Then wait out a minimum pulse width after clearing the bus
    (void) Timer_Sleep(0.450);
}

/* Waits for the LCD MCU to finish its tasks
 * Cannot be used until MCU is initialized
 */
void lcd_Await() {
    byte isBusy = 0;

    lcd_ReadMode;
    lcd_InstructionMode;

    do {
        // Signal a transfer and hold it on for PW_ED
        lcd_StartTransfer;
        (void) Timer_Sleep(0.320);

        // Read the response, then wait out the hold period of 20ns (tH)
        isBusy = PTH & 0b10000000;
        lcd_StopTransfer;
        (void) Timer_Sleep(0.020);
    } while (isBusy);
}

/* Writes data to the LCD bus
 * Using LCD_Addr and LCD_CGAddr sets which RAM is written
 */
void lcd_Data(byte data) {
    // Wait for the MCU to finish tasks
    lcd_Await();

    // Prepare the bus to submit an instruction
    lcd_WriteMode;
    lcd_DataMode;
    PTH = data;

    // Set up time (tAS) after RS/RW changes is 140ns minimum
    (void) Timer_Sleep(0.140);

    lcd_StartTransfer;
    (void) Timer_Sleep(0.450);
    lcd_StopTransfer;

    // the data bus and controls must be unchanged for 10ns after a transfer ends
    (void) Timer_Sleep(0.010);
    PTH ^= PTH;  // Clear Port H after use

    // Then wait out a minimum pulse width after clearing the bus
    (void) Timer_Sleep(0.450);
}

/* Configures the bus to write to DDRAM, the text displayed on the LCD
 *
 * Line 1 - 0x00 ... 0x13
 * Line 3 - 0x14 ... 0x27
 *  N/A*  - 0x28 ... 0x39
 * Line 2 - 0x40 ... 0x53
 * Line 4 - 0x54 ... 0x67
 *  N/A*  - 0x68 ... 0x7F
 * 
 * * These addresses are undefined, undisplayed and may mirror other addresses
 *   use them at your own risk
 */
void lcd_Addr(byte addr) {
    lcd_Inst(0x80 | addr & 0x7F);
}

void lcd_AddrXY(byte ix, byte iy) {
    byte addr = (3 - iy % 4) % 2 ? 0x00 : 0x40;
    addr += (3 - iy % 4) > 1 ? 0x00 : 0x14;
    addr += ix % 0x14;

    lcd_Addr(addr);
}

/* Writes a null-terminated shift-JIS string
 * The first 0x0f values are reserved for CGRAM characters
 */
void lcd_String(char const *straddr) {
    for (; *straddr; straddr++) {
        lcd_Data(*straddr);
    }
}

void lcd_StringXY(byte ix, byte iy, char const *const straddr) {
    lcd_AddrXY(ix, iy);
    lcd_String(straddr);
}

void lcd_DisplayControl(byte displayOn, byte cursorOn, byte blinkOn) {
    lcd_Inst(0x08 | (displayOn ? 0b100 : 0) | (cursorOn ? 0b10 : 0) | (blinkOn ? 1 : 0));
}

void lcd_Clear() {
    lcd_Inst(0x01);
}

void lcd_Home() {
    lcd_Inst(0x02);
}

void lcd_ShiftL() {
    lcd_Inst(0x18);
}

void lcd_ShiftR() {
    lcd_Inst(0x1C);
}

void lcd_CGAddr(byte addr) {
    lcd_Inst(0x40 | addr & 0x3F);
}

/* CGRAM is 6 bits of addressable memory for user-defined characters
 * Characters are stored row-by-row
 * Only the lower 5 bits (xxx11111) of each address are used to indicate the state of pixels
 * 1 corresponds to on, 0 to off
 * The rows count from top of the character to the bottom
 * A new character begins every 8 or 10 rows depending on the font selected
 * Upon using this command, lcd_Addr manipulates the CGRAM address
 */
void lcd_CGChar(byte cgAddr, char const *const cgData) {
    lcd_CGAddr(cgAddr);
    lcd_String(cgData);
    lcd_Addr(0x00);
}