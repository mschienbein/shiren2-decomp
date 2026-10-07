#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    unsigned char unk0;
} Obj;

extern unsigned char D_801429C0[16];
extern s32 func_800A2FFC(void *room, void *arg1);
extern void *func_800B221C(void *out);
extern void *func_800A33DC(void *out, void *rect);
extern s32 func_800AD714(Obj *obj, Pos *pos);
extern s32 func_800B5BDC(Pos *pos);
extern u32 func_800B1C6C(void *pos);
extern s32 func_800B61EC(void *pos);

s32 func_800AE03C(Obj *obj, void *room, s32 allowItem, s32 allowWater, Pos *out) {
    s32 i;
    s32 blocked;
    Pos pos;

    for (i = 0;; i++) {
        if (i >= 100) {
            return 0;
        }
        if (func_800A2FFC(room, D_801429C0)) {
            func_800B221C(&pos);
        } else {
            func_800A33DC(&pos, room);
        }
        if ((func_800AD714(obj, &pos) ^ 1) != 0) {
            continue;
        }
        if (allowItem == 0 && func_800B5BDC(&pos)) {
            continue;
        }
        if (allowWater == 0 && (func_800B1C6C(&pos) & 0x2000)) {
            continue;
        }
        if (obj->unk0 == 10) {
            Pos *cell = &pos;

            blocked = 0;
            if (func_800B1C6C(cell) & 0x2000) {
                blocked = 1;
            } else if (func_800B61EC(cell)) {
                blocked = 1;
            }
            if (blocked) {
                continue;
            }
        }
        *out = pos;
        return 1;
    }
}
