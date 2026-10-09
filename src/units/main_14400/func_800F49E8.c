#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xA];
    u8 idA;
} Obj_800F49E8;

s32 func_800F47F0(unsigned char i);
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);

char *func_800F49E8(Obj_800F49E8 *obj, char *dst) {
    func_80083C90(dst, func_80048480(func_800F47F0(obj->idA)));
    return dst;
}
