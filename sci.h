#ifndef _SCI_H_
#define _SCI_H_

double SCI0_Init(double busClock, double baudRate);

int SCI0_Read(char const *data);
byte SCI0_BlockRead(void);

void SCI0_BlockWrite(byte data);
void SCI0_WriteString(char const *stringAddr);

int SCI0_ReadString(char *const buffer, int bufferLength);

#endif