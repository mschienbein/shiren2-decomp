#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x8]; u8 field_8; } Obj80041BBC;
extern Obj80041BBC *D_801476B8;
u8 func_80041BBC(void) {
    return D_801476B8->field_8;
}
