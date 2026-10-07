#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 field_0; s32 field_4; s32 field_8; s32 field_C; } Desc800B36C4;
typedef struct { s32 x; s32 y; } Pos800B36C4;
void *func_800B3080(void *desc, void *pos);
void func_800B33BC(void *out, Desc800B36C4 *desc, u8 kind, s32 arg3);

void *func_800B36C4(void *out, Pos800B36C4 *pos, u8 kind, s32 arg3) {
    Desc800B36C4 desc;

    func_800B3080(&desc, pos);
    func_800B33BC(out, &desc, kind, arg3);
    return out;
}
