#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Direction;
typedef struct { u16 unk0, unk2; const void *unk4; Direction unk8; u8 unk9; } State;
extern const s32 D_80157FA8[]; /* Address-only view of the 0x30-byte vtable. */
extern const unsigned char D_80158A48[48];
static inline Direction *unpack_direction(Direction *direction, u8 packed) {
    direction->value = packed & 7;
    return direction;
}
State *func_800DF3C0(State *arg0, u8 *arg1) {
    Direction direction;
    arg0->unk4 = &D_80157FA8;
    arg0->unk0 = 2;
    arg0->unk4 = D_80158A48;
    arg0->unk9 = *arg1 >> 4;
    arg0->unk8 = *unpack_direction(&direction, *arg1);
    return arg0;
}
