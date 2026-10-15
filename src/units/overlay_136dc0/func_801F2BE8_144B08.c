#include "func_801F2BE8_144B08/types.h"

s32 func_801F2BE8_144B08(Obj801F63A8 *obj, s32 flag) {
    if (flag) {
        if (obj->field_14 != 1) {
            return 1;
        }
    } else if (obj->field_14 == 2) {
        return 1;
    }
    return 0;
}
