#include "common.h"

typedef unsigned char u8;
typedef short s16;
/* D_80158C98 entry 18 targets func_800E115C. */
typedef struct { u8 pad_00[0x90]; s16 this_adjust; s16 pad_92; s32 (*call)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_00[0x24]; VTable *field_24; } Object;

s32 func_800E2BD4(Object *object) {
    return object->field_24->call((u8 *)object + object->field_24->this_adjust, 1, 11, 0, 0);
}
