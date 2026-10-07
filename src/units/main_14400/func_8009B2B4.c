#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pair;

/* Partial view: only the selector word at +0x88 is known. */
typedef struct {
    char pad0[0x88];
    s32 field_88;
} Obj;

extern u8 D_80152890[];
extern u8 D_80152894[];
extern u8 D_801528A0[];

Pair func_8009B2B4(Obj *obj, Pair *in)
{
    Pair out;

    out.x = in->x;
    if (out.x == 0) {
        u8 index;

        if (obj->field_88 != 0) {
            index = D_80152894[in->y];
        } else {
            index = D_801528A0[in->y];
        }
        out.y = D_80152890[index] - 1;
    } else {
        out.y = in->y * 2;
    }
    return out;
}
