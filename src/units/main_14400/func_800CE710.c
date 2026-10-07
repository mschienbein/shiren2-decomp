#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xE];
    u8 field_E;
} Obj;

/* D_80154390/D_80154438/D_80154550 slot +0x24 returns an int count;
 * the byte-sized field is promoted, not a narrow function result. */
s32 func_800CE710(Obj *obj) {
    return obj->field_E;
}
