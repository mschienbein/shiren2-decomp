#include "common.h"

typedef unsigned char u8;

s32 func_800AFD08(void *table, void *obj);
void func_800AFCA8(void *table, u8 id);

void func_800AFC6C(void *table, void *obj) {
    u8 id = func_800AFD08(table, obj);

    if (id != 0xFF) {
        func_800AFCA8(table, id);
    }
}
