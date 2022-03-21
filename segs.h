#pragma ONCE

// for functions that want to add/kill the decimal point
typedef enum Segs_DPOption
{
  Segs_DP_OFF, Segs_DP_ON
} Segs_DPOption;

// for functions that specify the upper or lower full display
typedef enum Segs_LineOption
{
  Segs_LineTop, Segs_LineBottom
} Segs_LineOption;

// setup ports to speak with 7-segs
void Segs_Init(void);

// control segs manually
void Segs_Custom(byte Addr, byte Value);

// show HEX decoding (changes all of display to HEX, w/wo dp)
void Segs_Normal(byte Addr, byte Value, Segs_DPOption dp);

// deprecated after term 1202
// go code B (changes all of display to code b)
void Segs_CodeB(byte Addr, byte Value);

// show a 16-bit value on the upper or lower display
void Segs_16H(word Value, Segs_LineOption Line);

// show a 16-bit value on the upper or lower display with specified decimal points
void Segs_16HP(word Value, Segs_LineOption Line, Segs_DPOption Decimals[4]);

// show a decimal value on the first or second line of the 7-segs
void Segs_16D(word Value, Segs_LineOption Line);

// show a 16-bit value on the upper or lower display with specified decimal points
void Segs_16DP(word Value, Segs_LineOption Line, Segs_DPOption Decimals[4]);

// show the 8-bit value starting on the digit as addr (0-6)
void Segs_8H(byte Addr, byte Value);

// clear the display
void Segs_Clear(void);

// say Err on the appropriate line
void Segs_SayErr(Segs_LineOption Line);

// deprecated after term 1202
// say 'help' as best the 7-segs can show
void Segs_SayHelp(void);