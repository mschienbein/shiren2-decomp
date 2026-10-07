#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 kind;
    s32 value;
} Pair8009B0C0;

typedef struct {
    u8 pad0[0x34];
    Pair8009B0C0 field_34;
} Obj8009B0C0;

extern u8 D_801528C0[];
extern s32 D_801528C4[];

Pair8009B0C0 func_8009B178(void *self, Pair8009B0C0 *src);
void func_800488F0(Obj8009B0C0 *obj, Pair8009B0C0 *from, s32 arg2, Pair8009B0C0 *to);

void func_8009B0C0(Obj8009B0C0 *obj, Pair8009B0C0 *pair) {
    Pair8009B0C0 next;
    Pair8009B0C0 from;
    Pair8009B0C0 to;

    next = *pair;
    if (pair->kind == 0) {
        next.value = D_801528C4[D_801528C0[pair->value]];
    }
    from = func_8009B178(obj, &next);
    to = func_8009B178(obj, &obj->field_34);
    func_800488F0(obj, &from, 0, &to);
    obj->field_34 = next;
}
