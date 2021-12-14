#ifndef _LCD_H_
#define _LCD_H_

// Initializes the LCD
void lcd_Init(void);

// Write an instruction to the LCD
void lcd_Inst(byte val);

/* Waits for the LCD MCU to finish its tasks
 * Cannot be used until MCU is initialized
 */
void lcd_Await(void);

/* Writes data to the LCD bus
 * Using LCD_Addr and LCD_CGAddr sets which RAM is written
 */
void lcd_Data(byte val);

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
void lcd_Addr(byte addr);
void lcd_AddrXY(byte ix, byte iy);
void lcd_String(char const *straddr);
void lcd_StringXY(byte ix, byte iy, char const * const straddr);

void lcd_DisplayControl(byte displayOn, byte cursorOn, byte blinkOn);
void lcd_Clear(void);
void lcd_Home(void);
void lcd_ShiftL(void);
void lcd_ShiftR(void);

void lcd_CGAddr(byte addr);
void lcd_CGChar(byte cgAddr, char const * const cgData);

#endif