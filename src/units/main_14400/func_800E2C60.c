#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad_00[0x90];
    short adjust_90;
    short pad_92;
    s32 (*call_94)(void *obj, s32 mode, s32 a, u8 b, s32 c);
} VTable;
typedef struct {
    u8 pad_00[0x24];
    const VTable *field_24;
} Obj;

/* D_80158C98 and D_80159320 slot 0x94 both target func_800E115C. */
s32 func_800E2C60(Obj *obj) {
    return obj->field_24->call_94((char *)obj + obj->field_24->adjust_90, 1, 10, 0, 0);
}
