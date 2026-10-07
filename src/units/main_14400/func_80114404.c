#include "common.h"
typedef signed short s16;
typedef unsigned char u8;
typedef struct { unsigned char type; char pad[0xB]; u32 xC; } Ent;
/* 0x10-byte iterator: func_800CEBA0 stores and func_800CEC68 returns the current element at +0xC. */
typedef struct { s32 index; void *list; s32 reverse; void *current; } Iter;
extern char D_80147620[];
void *func_8011422C(void *a);
Iter *func_800CEB20(Iter *it, void *list);
s32 func_800CEBA0(Iter *it);
Ent *func_800CEC68(Iter *it);
s32 func_8010BA90(Ent *e, s16 v);
void func_801115D4(Ent *e, s32 v);
s32 func_800C5844(void *rng, u8 lo, u8 hi);
s32 func_8010E05C(Ent *e, s32 v);
void func_80114404(void *a, s32 v1, s32 v2, s32 lo, s32 hi) {
    Iter it;
    Ent *e;
    s32 amount;
    func_800CEB20(&it, func_8011422C(a));
    while (func_800CEBA0(&it)) {
        e = func_800CEC68(&it);
        switch (e->type) {
        case 3:
        case 4:
            func_8010BA90(e, (s16)v1);
            break;
        case 7:
            func_801115D4(e, (s16)v2);
            break;
        case 14:
            if (lo < 0) {
                amount = -(e->xC * (u8)func_800C5844(D_80147620, -lo, -hi) / 100);
                if (amount == 0) amount = -1;
            } else {
                amount = e->xC * (u8)func_800C5844(D_80147620, lo, hi) / 100;
                if (amount == 0) amount = 1;
            }
            func_8010E05C(e, amount);
            break;
        }
    }
}
