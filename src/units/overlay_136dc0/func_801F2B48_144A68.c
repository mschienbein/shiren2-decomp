#include "func_801F2B48_144A68/types.h"

s32 func_801F2B48_144A68(Obj801F63A8 *obj, void *actor) {
    s32 result = 1;

    switch (obj->field_00) {
    case 0:
        /* Mode 0 succeeds without movement or default-case notifications. */
        break;
    case 1:
        result = func_800E776C(actor, 10);
        break;
    default:
        func_800E039C(actor, 0);
        func_80049CB4(0x1F, actor);
        /* fallthrough */
    case 2:
        result = func_800E6170(actor, &obj->save.rect);
        break;
    }
    return result;
}
