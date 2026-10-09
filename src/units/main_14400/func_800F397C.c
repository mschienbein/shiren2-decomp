#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
typedef struct Owner {
    u8 pad_00[0x8C];
    Obj *collection_8C;
} Owner;
extern s32 func_800CD1FC(Obj *obj);

s32 func_800F397C(Owner *owner)
{
    return func_800CD1FC(owner->collection_8C);
}
