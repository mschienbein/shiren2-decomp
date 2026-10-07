#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xC]; s32 field_C; } Obj80112CD4;
s32 func_80112CD4(Obj80112CD4 *obj) {
    return obj->field_C;
}
