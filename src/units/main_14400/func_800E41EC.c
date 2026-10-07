#include "common.h"

typedef unsigned char u8;
s32 func_800A692C(void*, s32); void *func_800AC244(u8); s32 func_80049CB4(s32, ...); s32 func_800ADC90(void*, void*, void*);
s32 func_800E41EC(void *a, u8 *b){
    u8 id = 0;
    void *item;
    s32 r;
    s32 blocked = func_800A692C(a, 8) == 1;
    if (!blocked) {
        id = b[0xB];
        if (func_800A692C(a, 6)) id = 0xA1;
        else if (func_800A692C(a, 7)) id = 0xCC;
    }
    if (id == 0) return 0;
    item = func_800AC244(id);
    if (item == 0) return 0;
    func_80049CB4(6);
    r = func_800ADC90(item, a, a);
    func_80049CB4(7);
    return r;
}
