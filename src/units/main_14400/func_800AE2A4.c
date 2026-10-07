#include "common.h"
typedef struct { short offset; short pad; void (*func)(void *self, s32 mode); } VtEntry800AE2A4;
typedef struct { unsigned char pad0[8]; VtEntry800AE2A4 entry_8; } Vtable800AE2A4;
typedef struct { s32 field_0; s32 field_4; Vtable800AE2A4 *vtable; } Obj800AE2A4;
typedef struct { s32 field_0; s32 field_4; } Buf800AE2A4;
s32 func_800AE03C(void *obj, void *area, s32 allowItem, s32 allowWater, void *out);
s32 func_800AE18C(Obj800AE2A4 *obj, Buf800AE2A4 *buf);
s32 func_800AE2A4(Obj800AE2A4 *obj, s32 allowItem, s32 allowWater, void *area) {
    Buf800AE2A4 buf;
    if ((func_800AE03C(obj, area, allowItem, allowWater, &buf) ^ 1) != 0) {
        if (obj != 0) {
            obj->vtable->entry_8.func((char *)obj + obj->vtable->entry_8.offset, 3);
        }
        return 0;
    }
    return func_800AE18C(obj, &buf);
}
