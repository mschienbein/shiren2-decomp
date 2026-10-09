#include "common.h"

typedef struct {
    unsigned char unk00[0x29];
    unsigned char unk29;
} Object;

s32 func_800F4CF8(Object *object) {
    return object->unk29 != 0;
}
