#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
extern void *func_800A38FC(s32 size);
extern Obj *func_801016E0(Obj *p, s32 a);

Obj *func_801016A0(u8 kind, Obj *storage)
{
    if (storage != 0) {
        return func_801016E0(storage, kind);
    } else {
        return func_801016E0(func_800A38FC(0xC4), kind);
    }
}
