#include "common.h"

typedef struct {
    char pad0[0x20];
    s32 unit_20;
    char pad24[0x3B4 - 0x24];
    s32 ids_3B4[3];
    s32 count_3C0;
    s32 offsets_3C4[4];
    s32 starts_3D4[4];
} Obj80098E34;

s32 func_80098DD4(Obj80098E34 *obj, s32 id);

s32 func_80098E34(Obj80098E34 *obj, s32 pos) {
    s32 i;
    s32 unitPos;
    s32 id;
    s32 base;
    s32 size;

    if (pos >= 0) {
        id = 0;
        base = 0;
        unitPos = pos / obj->unit_20;
        for (i = 0; i < 3; i++) {
            if (unitPos < obj->starts_3D4[i + 1]) {
                id = obj->ids_3B4[i];
                base = obj->starts_3D4[i] * obj->unit_20;
                if (pos - base >= obj->offsets_3C4[i + 1] - obj->offsets_3C4[i]) {
                    return -1;
                }
                break;
            }
        }
        if (i >= 3) {
            return -1;
        }
        for (;;) {
            size = func_80098DD4(obj, id);
            if (size <= 0) {
                break;
            }
            base += size;
            if (pos < base) {
                return id;
            }
            id++;
        }
    }
    return -1;
}
