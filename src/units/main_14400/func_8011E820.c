#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad_00[0x90]; s16 delta; s16 pad_92; s32 (*call)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_00[0x1E]; u8 field_1E; u8 pad_1F[5]; VTable *field_24; } Obj;
extern u32 D_8013960C;
extern s32 func_800E04D0(Obj *obj);

/* Slot +0x54 supplies five pointers; only object is used by this target. */
void func_8011E820(void *self, void *event, Obj *object, void *direction, void *attacker) {
    if (object->field_1E & 0x7C) {
        D_8013960C <<= 1;
        if ((u8)func_800E04D0(object) == 0) {
            object->field_24->call((u8 *)object + object->field_24->delta, 1, 0x11, 0, 0);
            object->field_24->call((u8 *)object + object->field_24->delta, 0, 0xE, 0xFE, 0);
        } else {
            object->field_24->call((u8 *)object + object->field_24->delta, 0, 0x11, 0xFE, -1);
        }
        D_8013960C >>= 1;
    }
}
