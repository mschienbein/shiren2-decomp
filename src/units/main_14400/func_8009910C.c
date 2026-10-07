#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad0[0x20];
    s32 unit_20;
    u8 pad24[0x3C0 - 0x24];
    s32 count_3C0;
} Obj8009910C;
s32 func_80098E34(Obj8009910C *obj, s32 value);
s32 func_80099028(Obj8009910C *obj, s32 index);

s32 func_8009910C(Obj8009910C *obj, s32 limit) {
    s32 i = func_80098E34(obj, limit * obj->unit_20);
    s32 value;

    if (i < 0) {
        return i;
    }
    for (;;) {
        if (i >= obj->count_3C0) {
            return i;
        }
        value = func_80099028(obj, i + 1);
        if (value < 0) {
            return i;
        }
        if (limit < value) {
            return i;
        }
        i++;
    }
}
