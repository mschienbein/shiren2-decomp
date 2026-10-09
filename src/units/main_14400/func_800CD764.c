#include "common.h"
/* List vtable slots (D_80154390): +0x24 func_800CE710 s32 count(self); +0x54 func_800CE87C
 * void move(self, s32 source, s32 destination). */
typedef struct {
    unsigned char pad_00[0x20];
    short adjust_20;
    unsigned short reserved_22;
    s32 (*count_24)(void *self);
    unsigned char pad_28[0x50 - 0x28];
    short adjust_50;
    unsigned short reserved_52;
    void (*move_54)(void *self, s32 source, s32 destination);
} VTable;
typedef struct { void *pool0; VTable *field4; } Object;
extern s32 func_800CD5C0(Object *, void *);
s32 func_800CD764(Object *obj, s32 index, void *value) {
    if (func_800CD5C0(obj, value)) {
        s32 last = obj->field4->count_24((unsigned char *)obj + obj->field4->adjust_20) - 1;
        if (last > 0) {
            if (index != last) {
                obj->field4->move_54((unsigned char *)obj + obj->field4->adjust_50, last, index);
            }
        }
        return 1;
    }
    return 0;
}
