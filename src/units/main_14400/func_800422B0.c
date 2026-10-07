#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Vec2i;

u8 *func_800B4D80(Vec2i *pos);
s32 func_801158EC(u8 *entity);

s32 func_800422B0(s32 y, s32 x) {
    Vec2i pos;
    s32 result = 1;
    u8 *entity;

    pos.y = y;
    pos.x = x;
    entity = func_800B4D80(&pos);
    if (entity != 0 && *entity == 0x10) {
        switch (func_801158EC(entity)) {
            case 0:
                break;
            case 1:
                result = 2;
                break;
            case 2:
                result = 4;
                break;
        }
    }
    return result;
}
