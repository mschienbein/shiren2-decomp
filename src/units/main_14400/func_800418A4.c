#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Unit Unit;

extern Unit *func_800C5F60(void);
extern u8 func_800A8C00(void *actor);

s32 func_800418A4(void)
{
    return func_800A8C00(func_800C5F60());
}
