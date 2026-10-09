#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { u8 field_00; u8 kind_01; } Obj;
extern u8 D_80143094[];
extern u8 D_80145460[54][76];
extern Pair D_80143360;
extern Pair D_80143368[4];
extern u8 D_80143390;
extern s32 func_800AFD08(void *table, void *obj);

void func_800B4C64(Pair *pos, Obj *obj) {
    s32 outside = 0;
    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (!outside) {
        D_80145460[pos->x][pos->y] = func_800AFD08(D_80143094, obj);
        if (obj->kind_01 == 0xCE) {
            D_80143360 = *pos;
        }
        D_80143390 = (D_80143390 + 1) % 4;
        D_80143368[D_80143390] = *pos;
    }
}
