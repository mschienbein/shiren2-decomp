#include "common.h"
typedef struct { s32 x, y; } Pos;
typedef struct { s32 unk[4]; } Area;
typedef struct { short delta; short index; s32 (*fn)(void *, s32, s32, unsigned char, s32); } VEntry;
typedef struct { char pad[0x24]; VEntry *vtbl; } Obj;
extern u32 D_8013960C;
void *func_800B3080(Area *out, Pos *pos);
s32 func_800B2A14(Area *, s32);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32, ...);
s32 func_800A8FC8(s32 *, s32);
Obj *func_800A910C(s32 *);
s32 func_800E1CD4(Obj *, s32);
static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011C2E0(void *self, Pos *src, void *item) {
    Pos pos;
    Area area;
    Pos_copy(&pos, src);
    func_800B3080(&area, &pos);
    if (func_800B2A14(&area, 0)) {
        s32 it;
        func_80049CB4(0x11D, &pos);
        func_800B2A14(&area, 1);
        func_800498E4(0xDA);
        it = 0;
        D_8013960C *= 2;
        while (1) {
            Obj *obj;
            if (!func_800A8FC8(&it, 0x7C)) break;
            obj = func_800A910C(&it);
            if (func_800E1CD4(obj, 0x10)) {
                VEntry *e = &obj->vtbl[18];
                e->fn((char *)obj + e->delta, 1, 0x10, 0, 0);
            }
        }
        D_8013960C /= 2;
    } else {
        func_80049CB4(0x132);
        func_800498E4(0x223);
    }
}
