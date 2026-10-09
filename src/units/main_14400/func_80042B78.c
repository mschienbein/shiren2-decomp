#include "common.h"

typedef struct Unit Unit;

Unit *func_800C5F60(void);
s32 func_800E49F4(Unit *unit, s32 flag);

s32 func_80042B78(s32 flag) {
    return func_800E49F4(func_800C5F60(), flag != 0);
}
