#pragma ONCE

typedef enum MCP_Channel {
    MCP_ChannelA,
    MCP_ChannelB
} MCP_Channel;

typedef enum MCP_Gain {
    MCP_2xGain,
    MCP_NoGain
} MCP_Gain;

extern const word MCP_MAX;

void MCP_Init(void);
void MCP_Shutdown(MCP_Channel channel);
void MCP_Write(MCP_Channel channel, MCP_Gain gain, word data);