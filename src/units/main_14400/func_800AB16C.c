#include "common.h"

typedef unsigned char u8;
u8 func_800AB1C8(void *, u8, s32);
void *func_800AC244(u8);
void func_800AB35C(void *, s32);

void *func_800AB16C(void *table, u8 kind, s32 arg)
{
    void *obj = 0;
    u8 id = func_800AB1C8(table, kind, arg);

    if (id != 0) {
        obj = func_800AC244(id);
        func_800AB35C(obj, arg);
    }
    return obj;
}
