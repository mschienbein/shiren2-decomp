#include "common.h"

typedef unsigned char u8;

typedef void *(*Ctor800A86EC)(void);

extern Ctor800A86EC D_8015CD74[];
s32 func_800A3934(void *obj);

void *func_800A86EC(u8 kind) {
    void *obj = D_8015CD74[kind - 2]();

    if (func_800A3934(obj)) {
        obj = 0;
    }
    return obj;
}
