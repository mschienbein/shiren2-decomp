#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

char *func_800A7DE4(void *obj);
s32 func_800A4EFC(void *obj, void *dir);
void func_800A7F54(void *obj) {
    func_800A4EFC(obj, func_800A7DE4(obj));
}
