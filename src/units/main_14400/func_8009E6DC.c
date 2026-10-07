#include "common.h"
typedef struct { s32 field0, field4; } Pair;
typedef struct { unsigned char pad[0x34]; Pair field34; unsigned char pad3C[0x17]; unsigned char field53; } Object;
extern void func_800487C4(Object *);
void func_8009E6DC(Object *p, Pair *value) {
    if (p->field53) {
        s32 old = p->field34.field4;
        p->field34 = *value;
        if (p->field34.field4 != old) func_800487C4(p);
    }
}
