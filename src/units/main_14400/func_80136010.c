#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

extern s32 func_80112674(void *self, s32 kind);

s32 func_80136010(void *self, s32 kind) {
    if (func_80112674(self, kind) != 0) {
        return 1;
    }
    return kind == 0x21;
}
