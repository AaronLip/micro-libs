#pragma ONCE

typedef enum PWM_ClockSource { PWM_ClockPrescaler, PWM_ClockScaler } PWM_ClockSource;
typedef enum PWM_Polarity { PWM_PolarityLow, PWM_PolarityHigh } PWM_Polarity;
typedef enum PWM_Alignment { PWM_AlignLeft, PWM_AlignCenter } PWM_Alignment;
typedef enum PWM_Width { PWM_Width8, PWM_Width16 } PWM_Width;

typedef enum PWM_Clock { PWM_ClockA, PWM_ClockB } PWM_Clock;

typedef enum PWM_Channel {
    PWM_Channel0,
    PWM_Channel1,
    PWM_Channel2,
    PWM_Channel3,
    PWM_Channel4,
    PWM_Channel5,
    PWM_Channel6,
    PWM_Channel7,
} PWM_Channel;

typedef enum PWM_Prescale {
    PWM_Prescale1,
    PWM_Prescale2,
    PWM_Prescale4,
    PWM_Prescale8,
    PWM_Prescale16,
    PWM_Prescale32,
    PWM_Prescale64,
    PWM_Prescale128
} PWM_Prescale;

typedef byte PWM_Scale;
typedef word PWM_Channel_Period;
typedef word PWM_Channel_Duty;

typedef enum PWM_Type {
    PWM_Interval,  // One PWM event per duration of input value
    PWM_Period,    // Two PWM events per duration of input value
    PWM_Frequency  // Twice the input value in events per second
} PWM_Type;
typedef struct PWM_Quantity { PWM_Type type; double value; } PWM_Quantity;

typedef enum PWM_Note {
    PWM_NoteA,
    PWM_NoteB,
    PWM_NoteC,
    PWM_NoteD,
    PWM_NoteE,
    PWM_NoteF,
    PWM_NoteG
} PWM_Note;

typedef struct PWM_Colour { byte R; byte G; byte B; } PWM_Colour;

void PWM_ClockSource_Set(PWM_Channel channel, PWM_ClockSource source);
void PWM_Polarity_Set(PWM_Channel channel, PWM_Polarity polarity);

void PWM_Clock_Init(PWM_Clock clock, PWM_Prescale prescale, PWM_Scale scale);
void PWM_Channel_Init(
    PWM_Channel channel,
    PWM_Width width,
    PWM_Channel_Period period,
    PWM_Channel_Duty duty,
    PWM_ClockSource source,
    PWM_Polarity polarity
);

void PWM_Duty(PWM_Channel channel, PWM_Channel_Duty duty);

void PWM_Colour_Init(void);
void PWM_Colour_Set(PWM_Colour colour);

void PWM_Backlight_Init(PWM_Channel_Duty duty);

void PWM_Backlight_Set(PWM_Channel_Duty duty);