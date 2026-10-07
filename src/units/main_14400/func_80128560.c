#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xD]; u8 field_D; u8 field_E; } Obj80128560;
char *func_800A8498(s32 id, s32 variant);
char *func_80083C90(char *dst, char *src);
void *func_80128560(Obj80128560 *obj, void *dst) {
    func_80083C90(dst, func_800A8498(obj->field_D, obj->field_E));
    return dst;
}
