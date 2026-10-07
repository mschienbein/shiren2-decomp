#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C. */
typedef struct { void *room; s32 opaque[3]; } RoomState;

extern RoomState D_80143434;
extern s32 func_800A31C8(void *rect, void *pos);

s32 func_800B61EC(void *pos) {
    s32 result;

    if (D_80143434.room != 0) {
        result = func_800A31C8(D_80143434.room, pos);
    } else {
        result = 0;
    }
    return result;
}
