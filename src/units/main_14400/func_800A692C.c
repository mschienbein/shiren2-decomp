#include "common.h"
extern void *func_800B31E8(void *object, s32 code);
s32 func_800A692C(void *object, s32 code) {
    return func_800B31E8(object, (unsigned char)code) != 0;
}
