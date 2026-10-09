#include "common.h"

typedef struct Unit {
    unsigned char pad_00[0x54];
    unsigned char field_54;
    unsigned char pad_55[3];
    struct Unit *field_58;
} Unit;
extern s32 func_800F1024(Unit *obj);
extern Unit *func_800FD4B4(Unit *obj);
extern s32 func_800E7104(Unit *obj);

s32 func_800FD66C(Unit *obj) {
    if (func_800F1024(obj) && func_800FD4B4(obj)) {
        obj->field_54 |= 4;
        obj->field_58 = 0;
        return 0;
    }
    return func_800E7104(obj);
}
