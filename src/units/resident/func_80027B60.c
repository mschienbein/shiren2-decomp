#include "controller_command_types.h"

void func_80027B60(u8 command)
{
    u8 *cursor;
    ControllerCommand request;
    s32 channel;

    for (channel = 14; channel >= 0; channel--) {
        D_80040FD0.words[channel] = 0;
    }
    D_80040FD0.command_status = 1;
    cursor = (u8 *)&D_80040FD0;

    request.dummy = 0xFF;
    request.transmit_count = 1;
    request.receive_count = 3;
    request.command = command;
    request.type_low = 0xFF;
    request.type_high = 0xFF;
    request.status = 0xFF;
    request.end = 0xFF;

    for (channel = 0; channel < D_80039008; channel++) {
        __builtin_memcpy(cursor, &request, sizeof(request));
        cursor += sizeof(request);
    }
    *cursor = 0xFE;
}
