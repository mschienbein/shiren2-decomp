#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct Obj Obj;

s32 func_800E04D0(Obj *obj);
void func_800E03BC(Obj *self, s32 level);

/* Adjust the 0..3 level by delta, clamping to the valid range. */
void func_800E046C(Obj *obj, s32 delta) {
    s8 level = func_800E04D0(obj) + delta;

    if (level < 0) {
        level = 0;
    } else if (level >= 4) {
        level = 3;
    }
    func_800E03BC(obj, (u8)level);
}
