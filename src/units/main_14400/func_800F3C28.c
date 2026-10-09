#include "common.h"

typedef struct {
    unsigned char unk00[0x7C];
    unsigned short unk7C;
} Object;

s32 func_800F3C28(Object *object) {
    return (object->unk7C >> 9) & 1;
}
