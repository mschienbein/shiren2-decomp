#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x8]; s32 index; u8 padC[0x5C]; } Slot80131570;
typedef struct { Slot80131570 *slot; s32 pad4; s32 unk8; } Obj80131570;
extern Slot80131570 D_801E00EC[];
s32 func_80131F04(s32 kind, void *obj);
void func_80131570(Obj80131570 *obj, s32 index) {
    Slot80131570 *slot = &D_801E00EC[index];
    obj->slot = slot;
    slot->index = index;
    obj->unk8 = func_80131F04(0x201, obj);
}
