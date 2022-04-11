#include <hidef.h>
#include <derivative.h>

#include "mcp4218.h"

#define MCP_Max 0x3ff

#define MCP_Select PTM_PTM0 = 0;
#define MCP_Unselect PTM_PTM0 = 1;

#define MCP_AwaitWrite while (!SPI0SR_SPTEF);
#define MCP_AwaitRead while (!SPI0SR_SPIF);

extern const word MCP_MAX = 0x3ff;

void MCP_RawWrite(word rawPayload) {
    volatile byte discard;

    MCP_Select;

    MCP_AwaitWrite;
    SPI0DR = (rawPayload & 0xff00) >> 8;

    MCP_AwaitWrite;
    SPI0DR = rawPayload & 0x00ff;

    MCP_AwaitRead;
    discard = SPI0DR;

    MCP_AwaitRead;
    discard = SPI0DR;

    MCP_Unselect;
}

#undef MCP_Select

#undef MCP_AwaitWrite
#undef MCP_AwaitRead

void MCP_Shutdown(MCP_Channel channel) {
    MCP_RawWrite(channel << 15);
}

void MCP_Init() {
    SPI0CR1 = 0b01010000;
    SPI0CR2 = 0;

    SPI0BR = 0;

    // Configure M0 as output
    DDRM_DDRM0 = 1;

    // Turn off both output channels asap
    MCP_Shutdown(MCP_ChannelA);
    MCP_Shutdown(MCP_ChannelB);
}

#undef MCP_Unselect

void MCP_Write(MCP_Channel channel, MCP_Gain gain, word data) {
    word payload = ((data & 0x3ff) << 2);
    payload |= 1 << 12;  // Turn channel on from shutdown
    payload |= gain << 13;
    payload |= channel << 15;

    MCP_RawWrite(payload);
}