#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
extern u8 D_80145460[54][76];
extern Pair D_80143360;
extern Pair D_80143368[4];
extern u8 D_80143390;

void func_800B4C64(Pair *pos, Item *obj) {
    s32 outside = 0;
    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (!outside) {
        D_80145460[pos->x][pos->y] = func_800AFD08(&D_80143094, obj);
        if (obj->field01 == 0xCE) {
            D_80143360 = *pos;
        }
        D_80143390 = (D_80143390 + 1) % 4;
        D_80143368[D_80143390] = *pos;
    }
}
