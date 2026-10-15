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
            /* ODD_C: ordinary type-0x10 map-cell display class: func_801158EC
             * returns 0 when func_8011575C's global flag is clear, retaining
             * display flags 1 for func_80061FB8. This label shapes the compare
             * tree: removing it changes 15 overlapping words and shrinks text
             * from 136 to 124 bytes in this profile. */
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
