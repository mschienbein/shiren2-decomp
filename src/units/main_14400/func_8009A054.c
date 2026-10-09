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
typedef struct { void *container; void *item; } Pair;
extern Pair func_80098018(Obj *, s32);

/* Struct return: the hidden result pointer is forwarded to func_80098018. */
Pair func_8009A054(Obj *o, s32 n) {
    if (o->unk2CC != 0 && o->unk2D0 == 0) {
        if (n == 0) {
            return func_80098018(o, o->vtbl[15].fn((char *)o + o->vtbl[15].delta, o->unk34));
        } else {
            return func_80098018(o, -1);
        }
    } else {
        return func_80098018(o, o->unk2D4[n]);
    }
}
