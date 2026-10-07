#include "common.h"

typedef unsigned char u8;

typedef struct { s32 index; s32 unk4; } Iter;
typedef struct { u8 pad[0xA0]; s32 xA0; } Monster;
typedef struct { u8 pad[0xA0]; s32 xA0; } Item;
s32 func_800A9070(Iter *, s32);
void *func_800A910C(Iter *);
s32 func_800E0F40(void *obj);
void func_80108660(void *unused, s32 kind, u8 filter, s32 value) {
    Iter it;
    Iter *iter = &it;
    it.index = 0;
    while (func_800A9070(iter, kind)) {
        void *e = func_800A910C(iter);
        Monster *m = e;
        Item *item = e;
        if (filter != 0 && filter != (u8)func_800E0F40(e)) continue;
        if (kind == 0x51) item->xA0 = value; else m->xA0 = value;
    }
}
