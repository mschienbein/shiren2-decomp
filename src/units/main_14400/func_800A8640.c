#include "common.h"

extern void *(*D_8015CD50[])(unsigned char);
extern s32 func_800A3934(void *);

void *func_800A8640(s32 type, s32 value) {
    void *object = D_8015CD50[(unsigned char)type - 0x57]((unsigned char)value);
    if (func_800A3934(object) != 0) {
        object = 0;
    }
    return object;
}
