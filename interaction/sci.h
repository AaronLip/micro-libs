#pragma ONCE

double SCI0_Init(double busClock, double baudRate);

int SCI0_Read(char *data);
char SCI0_BlockRead(void);

int SCI0_Write(char data);
void SCI0_BlockWrite(char data);
void SCI0_WriteString(char const *stringAddr);

int SCI0_ReadString(char *buffer, int bufferLength);