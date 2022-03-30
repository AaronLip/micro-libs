#pragma ONCE

double SCI0_Init(double busClock, double baudRate);

int SCI0_Read(byte *data);
byte SCI0_BlockRead(void);

int SCI0_Write(byte data);
void SCI0_BlockWrite(byte data);
void SCI0_WriteString(char const *stringAddr);

int SCI0_ReadString(char *const buffer, int bufferLength);