#include "common.h"

typedef unsigned char u8;

/* One of the bank's eight 0x20-byte entries at offset 0x0 (func_8008BF14 passes
 * bank + index * 0x20); func_8008CEBC links it into the slot (+0x74) and bumps +0x1. */
typedef struct {
    u8 unk0;
    u8 users;
    u8 pad2[0x20 - 0x2];
} Info8008D07C;

typedef struct {
    u8 data[0x78];
} Slot8008D07C;

typedef struct {
    Info8008D07C infos[8];
    Slot8008D07C slots[8];
    u8 pad4C0[0x4C4 - 0x4C0];
    s32 count;
} Obj8008D07C;

s32 func_8008D220(void *bank);
s32 func_8008CEBC(Slot8008D07C *slot, Info8008D07C *info);
void *func_8008CED4(Slot8008D07C *slot);
void func_8008C9F4(Slot8008D07C *slot);

s32 func_8008D07C(Obj8008D07C *obj, Info8008D07C *info) {
    s32 index = func_8008D220(obj);

    if (index != -1) {
        Slot8008D07C *slot = &obj->slots[index];

        if (!func_8008CEBC(slot, info)) {
            return index;
        }
        func_8008CED4(slot);
        func_8008C9F4(slot);
        obj->count--;
        index = -1;
    }
    return index;
}
