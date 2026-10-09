#include "common.h"
typedef unsigned short u16;
typedef unsigned char u8;
extern u8 func_800AE98C(u8 *);
extern char *func_80048480(u16);
extern u16 *D_801538B4[];
char *func_800ACC90(u8 *obj) {
    u8 index = *obj;
    u8 offset = func_800AE98C(obj);
    return func_80048480(D_801538B4[index][offset]);
}
