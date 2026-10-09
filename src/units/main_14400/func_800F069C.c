#include "common.h"
typedef struct { unsigned char pad_0[0x9A]; unsigned short flags_9A; } Object;
extern s32 func_800E20CC(void *arg0);
s32 func_800F069C(void *arg) {
    Object *p = arg;
    s32 result = 0;
    if (func_800E20CC(p) == 0) {
        result = 1;
        if (p->flags_9A & 0x40) result = 0;
    }
    return result;
}
