#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x20]; s32 field20; u8 pad24[0x390]; s32 starts3B4[8]; s32 counts3D4[3]; } Object;
typedef Object Obj80098E34;
extern s32 func_80098DD4(Obj80098E34 *obj, s32 id);

s32 func_80098F54(Object *self, s32 index) {
    s32 start, total, group;
    if (index < 0) return -1;
    start = 0;
    total = 0;
    for (group = 0; group < 3; group++) {
        if (index < self->starts3B4[group + 1]) {
            start = self->starts3B4[group];
            total = self->counts3D4[group] * self->field20;
            break;
        }
    }
    if (group >= 3) return -1;
    for (;;) {
        s32 size;
        if (start >= index) break;
        size = func_80098DD4(self, start);
        total += size;
        if (size <= 0) return -1;
        start++;
    }
    return total;
}
