#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern u16 *D_801537B8[];
extern u16 *D_801538B4[];
u8 func_800AE98C(u8 *obj);
char *func_80048480(u16 id);

char *func_800ACCDC(u8 *obj) {
    s32 kind = *obj;
    u8 index = func_800AE98C(obj);
    u16 *table = D_801537B8[kind];

    if (table == 0) {
        table = D_801538B4[kind];
    }
    return func_80048480(table[index]);
}
