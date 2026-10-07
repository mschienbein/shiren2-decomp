#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_80142EF7;
void *func_800ABEE4(u8 index);
void *func_800AB16C(void *handle, u8 value, s32 mode);
void *func_800AADF8(s8 id, u8 value) {
    if (id == -1) {
        id = D_80142EF7;
    }
    id--;
    return func_800AB16C(func_800ABEE4(id), value, 2);
}
