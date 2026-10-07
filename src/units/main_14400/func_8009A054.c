#include "common.h"

typedef struct { short delta; short index; s32 (*fn)(void *, void *); } VEntry;
typedef struct {
    char pad0[0x34];
    char unk34[0x18];   /* 0x34 */
    VEntry *vtbl;       /* 0x4C */
    char pad50[0x27C];
    s32 unk2CC;         /* 0x2CC */
    s32 unk2D0;         /* 0x2D0 */
    signed char unk2D4[0x15]; /* 0x2D4 */
} Obj;
extern void func_80098018(void *, Obj *, s32);

void *func_8009A054(void *self, Obj *o, s32 n) {
    if (o->unk2CC != 0 && o->unk2D0 == 0) {
        if (n == 0) {
            func_80098018(self, o, o->vtbl[15].fn((char *)o + o->vtbl[15].delta, o->unk34));
        } else {
            func_80098018(self, o, -1);
        }
    } else {
        func_80098018(self, o, o->unk2D4[n]);
    }
    return self;
}
