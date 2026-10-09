#include "common.h"
extern void *(*D_8015CC4C[])(unsigned char);
extern s32 func_800A3934(void *);
void *func_800A85A0(unsigned char kind, unsigned char value) {
    void *result = D_8015CC4C[kind - 0x18](value);
    if (func_800A3934(result)) result = 0;
    return result;
}
