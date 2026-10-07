#include "common.h"

extern s32 D_801585F8[];
extern void *func_800DA904(void *obj, s32 kind, unsigned char *params);

void **func_800DC448(void **object, unsigned char *value) {
    func_800DA904(object, 0x18, value);
    object[1] = D_801585F8;
    return object;
}
