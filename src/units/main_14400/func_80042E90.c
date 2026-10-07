#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void func_80062804(u32 mode, s32 id, s32 arg2, s32 arg3);
void func_80042E90(u8 *id) {
    func_80062804(1, *id, 0, 0);
}
