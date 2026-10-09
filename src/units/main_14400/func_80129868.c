#include "common.h"

typedef unsigned char u8;
typedef struct Record { u8 bytes[6]; } Record;
typedef struct Resource {
    u8 pad_00[0x1C];
    Record *records_1C;
} Resource;
typedef struct CommandState {
    u8 pad_00[0x74];
    Resource *resource_74;
    u8 pad_78[0xC];
    Record *selected_84;
} CommandState;

u8 *func_80129868(CommandState *state, u8 *cursor)
{
    s32 index = *cursor++;
    if (index >= 0x80) {
        index &= 0x7F;
        index <<= 8;
        index |= *cursor++;
    }
    state->selected_84 = &state->resource_74->records_1C[index];
    return cursor;
}
