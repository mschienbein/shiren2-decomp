#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0x24]; const void *field_24; } Object;
typedef struct { u8 field_00[0x104]; Object *field_104; } World;
extern World *D_801476B8;
extern const u8 D_80158C98[];
extern void func_800EBCD0(World *object);
extern void func_800A38A0(Object *object, s32 flags);
extern void func_800A3918(Object *object);
void func_800E016C(Object *object, s32 flags) {
    s32 selected = 0;
    object->field_24 = D_80158C98;
    if (D_801476B8->field_104) selected = object == D_801476B8->field_104;
    if (selected) func_800EBCD0(D_801476B8);
    func_800A38A0(object, 0);
    if (flags & 1) func_800A3918(object);
}
