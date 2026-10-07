#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x9];
    u8 info_9;
    u8 padA[0x12];
    u16 flags_1C;
} Obj_800B5900;

extern s32 func_800B1DF8(void *pos);
extern u32 func_800B1C6C(void *pos);
extern void *func_800B4928(void *pos);
extern s32 func_800A58B8(Obj_800B5900 *obj);
extern s32 func_800B58E4(s32 mode, s32 arg, s32 info, s32 value);

s32 func_800B5900(void *pos, s32 mode, s32 arg, void **out) {
    Obj_800B5900 *obj;
    s32 blocked;
    s32 info;

    if (func_800B1DF8(pos) != 0) {
        return 0;
    }
    switch ((u8)mode) {
        case 1:
            if (!(func_800B1C6C(pos) & 0x2100)) {
                return 0;
            }
            break;
        case 2:
            blocked = (func_800B1C6C(pos) & 0xE100) && !(func_800B1C6C(pos) & 0x2100);
            if (blocked) {
                return 0;
            }
            break;
    }
    obj = func_800B4928(pos);
    *out = obj;
    if (obj != 0) {
        if (((obj->flags_1C & 1) ^ 1) != 0) {
            info = obj->info_9 & 0xF;
            if (func_800B58E4(mode, arg, info, func_800A58B8(obj)) != 0) {
                return 0;
            }
        }
    }
    *out = 0;
    return 1;
}
