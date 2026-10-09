#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;

typedef struct { u8 kind; u8 level; } Pick;
typedef struct { u8 pad0[0x1F]; u8 kind; u8 pad20[4]; const void *vtbl; u8 pad28[0x4D]; u8 level; } Obj;
extern const unsigned char D_8015AE28[192];

extern u8 *D_80148410[];
void *func_800EFC70(void *obj, s32 arg1, u8 arg2);
void func_800AA700(u8 *kind, u8 *level, u8 *table);
Obj *func_800FF480(Obj *obj, u8 level) {
    Pick pick;
    s32 i;
    s32 active;
    pick.level = level;
    func_800EFC70(obj, 0x32, level);
    obj->vtbl = D_8015AE28;
    active = D_80142F18.mode != 0x4F;
    if (active) {
        for (i = 0; i < 100; i++) {
            func_800AA700(&pick.kind, &pick.level, D_80148410[pick.level - 1]);
            if (pick.kind != 0) {
                break;
            }
        }
        if (i == 100) {
            pick.kind = 0x1D;
            pick.level = 1;
        }
        obj->kind = pick.kind;
        obj->level = pick.level;
    }
    return obj;
}
