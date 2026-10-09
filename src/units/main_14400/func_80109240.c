#include "common.h"

typedef struct { unsigned char unknown00[0x24]; void *field24; unsigned char unknown28[0x8c]; void *fieldb4; } Object;
extern char D_80159130[], D_80159150[], D_8015C690[];
extern s32 func_800EE598(Object *);
extern void func_800E016C(Object *, s32);
extern void func_800A3918(Object *);
void func_80109240(Object *object, s32 flags) {
    object->fieldb4 = D_80159130;
    object->field24 = D_8015C690;
    func_800EE598(object);
    object->fieldb4 = D_80159130;
    object->field24 = D_80159150;
    func_800E016C(object, 0);
    if (flags & 1) func_800A3918(object);
}
