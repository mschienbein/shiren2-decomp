#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;

/* 12-byte effect pair record (see func_8007BE1C): ids[0] is the 0x162 unit,
 * ids[1] the 0x13A unit, -1 when released. */
typedef struct Entry {
    u8 active;
    u8 timer;
    u8 pad2[2];
    s32 ids[2];
} Entry;

/* 0xB0-byte unit record returned by func_8007946C (side 4 = D_801DD378). */
typedef struct Unit {
    u8 pad0[2];
    s16 kind;
    u8 pad4[0xA];
    u16 angle_0E;
    u8 pad10[0xC];
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 rotation_28;
    u8 pad2C[9];
    u8 field_35;
    u8 field_36;
    u8 pad37;
    s32 field_38;
    u16 type_3C;
    u8 pad3E[8];
    u8 alpha_46;
    u8 pad47[0x69];
} Unit;

extern Entry D_801A75F0[16];

Unit *func_8007946C(s32 side, s32 slot);
void func_8007935C(s32 i);
s32 func_8007BF38(s32 arg0);

/* Animates the paired 0x162/0x13A effect units of each active entry through
 * their 9-frame timer, releasing each unit on frame 8 and the entry once both
 * are released. */
void func_8007BF7C(void) {
    s32 i;
    s32 k;
    Entry *entry;
    Unit *unit;

    for (i = 0; i < 16; i++) {
        entry = &D_801A75F0[i];
        if (entry->active != 1)
            continue;
        for (k = 0; k < 2; k++) {
            if (entry->ids[k] == -1)
                continue;
            unit = func_8007946C(4, entry->ids[k]);
            if (unit->kind == -1)
                continue;
            switch (unit->type_3C) {
            case 0x162:
                switch (entry->timer) {
                case 0:
                    unit->scaleX = 1.0f;
                    unit->scaleY = 0.4f;
                    unit->scaleZ = 1.0f;
                    unit->alpha_46 = 77;
                    unit->field_38 = 0;
                    break;
                case 1:
                case 2:
                    unit->scaleX = 0.85f;
                    unit->scaleY = 0.7f;
                    unit->alpha_46 = 115;
                    break;
                case 3:
                case 4:
                    unit->scaleX = 0.7f;
                    unit->scaleY = 1.0f;
                    unit->alpha_46 = 153;
                    break;
                case 5:
                case 6:
                    unit->scaleX = 0.55f;
                    unit->scaleY = 0.7f;
                    unit->alpha_46 = 115;
                    break;
                case 7:
                    unit->scaleX = 0.4f;
                    unit->scaleY = 0.4f;
                    unit->alpha_46 = 77;
                    break;
                case 8:
                    func_8007935C(entry->ids[k]);
                    entry->ids[k] = -1;
                    break;
                }
                break;
            case 0x13A:
                switch (entry->timer) {
                case 0:
                    unit->rotation_28 = 4.712389f;
                    unit->field_36 = 2;
                    unit->field_35 = 0;
                    unit->field_38 = 0;
                    unit->alpha_46 = 77;
                    unit->angle_0E += 4;
                    unit->scaleX = 0.5f;
                    unit->scaleY = 0.5f;
                    unit->scaleZ = 0.5f;
                    break;
                case 1:
                case 2:
                    unit->alpha_46 = 115;
                    break;
                case 3:
                case 4:
                    unit->alpha_46 = 153;
                    break;
                case 5:
                case 6:
                    unit->alpha_46 = 115;
                    break;
                case 7:
                    unit->alpha_46 = 77;
                    break;
                case 8:
                    func_8007935C(entry->ids[k]);
                    entry->ids[k] = -1;
                    break;
                }
                break;
            }
        }
        entry->timer++;
        if (entry->ids[0] == -1 && entry->ids[1] == -1)
            func_8007BF38(i);
    }
}
