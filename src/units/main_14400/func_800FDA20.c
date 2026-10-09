#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x10]; short offset_10; short pad_12; s32 (*method_14)(u8 *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; u8 pad_28[0x62]; u8 field_8A; } Actor;
typedef Actor Object;
typedef Actor Obj_80049414;
extern s32 func_800E20CC(void *arg0);
extern s32 func_800F3358(Actor *actor);
extern void *func_800A6CF0(Object *);
extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
extern s32 func_80049CB4(s32 id, ...);
s32 func_800FDA20(Actor *actor) {
    Actor *target;
    s32 count;
    if ((func_800E20CC(actor) ^ 1) != 0) return func_800F3358(actor);
    target = func_800A6CF0(actor);
    if (func_800E1CC4(actor, 2)) count = 1;
    else count = actor->field_8A;
    while (count-- > 0) {
        if (!target || (target->field_24->method_14((u8 *)target + target->field_24->offset_10) ^ 1) != 0) func_800F3358(actor);
        func_80049CB4(12, 0);
        func_80049CB4(2);
    }
    return 1;
}
