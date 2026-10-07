#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x0; s32 x4; } Iter;
extern u8 D_80142F1B;
extern void *D_801476B8;
s32 func_800A8FC8(Iter *it, s32 kind);
void *func_800A910C(Iter *it);
s32 func_80049CB4(s32 id, ...);
void *func_800E8A68(void *o, u8 kind);
s32 func_8010EBA4(void *s);
void func_800E9F14(void *o, s32 flag);
void func_800C7830(void) {
    Iter it;
    Iter *ip = &it;
    void *o;
    void *s;
    ip->x0 = 0;
    while (1) {
        if (!func_800A8FC8(ip, 0xC)) break;
        o = func_800A910C(ip);
        func_80049CB4(0x1F, o);
        s = func_800E8A68(o, 3);
        if (s != 0 && func_8010EBA4(s)) func_80049CB4(0x8D, o, s);
        func_800E9F14(o, ((D_80142F1B >> 2) & 1) ^ 1);
    }
    func_80049CB4(0x89, D_801476B8);
}
