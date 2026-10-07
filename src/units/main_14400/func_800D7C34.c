#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; s32 unk4; } Iter;
typedef struct { u8 unk0; u8 unk1; u8 pad2[0xA]; s8 unkC; u8 unkD; u8 unkE; } Obj;
void *func_800B07F0(void *iter);
s32 func_800B0808(Iter *);
Obj *func_800B0864(Iter *);
static __inline__ s32 isMatch(Obj *obj, u8 a, u8 b) {
    return obj->unk1 == 0xF1 && (obj->unkC & 4) && obj->unkD == a && obj->unkE == b;
}
void func_800D7C34(u8 a, u8 b) {
    Iter it;
    Obj *obj;

    func_800B07F0(&it);
    while (func_800B0808(&it)) {
        obj = func_800B0864(&it);
        if (isMatch(obj, a, b)) {
            obj->unkC &= ~4;
        }
    }
}
