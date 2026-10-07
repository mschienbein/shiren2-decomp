#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 pad0[0xC];
    s32 target;
    u8 pad10[0x84];
    s32 time;
    u8 pad98[0x25];
    u8 volume;
    u8 padBE[0x16];
    u8 step;
    u8 padD5[0x4];
    u8 frac;
    u8 falling;
} Obj;

void func_8012B8B8(Obj *obj)
{
    u8 step = obj->step;
    u32 sum;

    do {
        sum = obj->frac + step;
        obj->time += 0x100;
        if (sum < 0x40) {
            obj->frac = sum;
        } else {
            obj->frac = sum & 0x3F;
            sum >>= 6;
            if (obj->falling == 0) {
                obj->volume += sum;
                if ((s8)obj->volume < 0) {
                    obj->volume = 0x7F;
                    obj->falling = 1;
                }
            } else {
                obj->volume -= sum;
                if ((u8)(obj->volume - 1) >= 0x7F) {
                    obj->volume = 0;
                    obj->falling = 0;
                }
            }
        }
    } while (obj->time - obj->target < 0);
}
