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

extern void Timer_Init(dword busClock, Timer_Prescale prescale, word initialOffset, word enableInt, Timer_PinAction pinAction);
extern void Timer_Sleep(dword milliseconds);
extern double Timer_Cycles(dword delayMilliseconds);