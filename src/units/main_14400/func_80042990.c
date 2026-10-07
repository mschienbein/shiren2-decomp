#include "common.h"
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C.
 * The +0xC word is the splat label D_80143440, read here. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern RoomState D_80143434;
s32 func_80042990(void) { return D_80143434.opaque[2] != 0; }
