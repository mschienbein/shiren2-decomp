#include "common.h"
typedef unsigned char u8;
typedef struct Obj Obj;
extern void *func_800A38FC(s32 size);
extern Obj *func_800FFCE0(Obj *obj, u8 kind);
Obj *func_800FFCA0(u8 kind, Obj *obj) {
    if (obj != 0) return func_800FFCE0(obj, kind);
    return func_800FFCE0(func_800A38FC(0xA0), kind);
}
