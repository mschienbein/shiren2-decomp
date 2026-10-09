#include "common.h"

typedef unsigned char u8;
extern s32 func_800AD468(u32 bit);
extern char *func_800ACCDC(u8 *object);
extern char *func_800ACC90(void *object);

char *func_800AC990(void *object)
{
    char *name;
    if (func_800AD468(((u8 *)object)[1]) == 0) {
        name = func_800ACCDC(object);
    } else {
        name = func_800ACC90(object);
    }
    return name;
}
