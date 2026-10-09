#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Current-room state (16 bytes): room/rectangle pointer at +0, three words at +4..+0xC. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern RoomState D_80143434;

s32 func_800B2048(void *unit)
{
    s32 result = 0;

    if (unit != 0) {
        result = D_80143434.room == unit;
    }
    return result;
}
