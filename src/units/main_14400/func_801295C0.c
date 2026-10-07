#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* Parameter-only state prefix for the command handler's flag at +0xB7. */
typedef struct { u8 pad0[0xB7]; u8 unkB7; } Obj801295C0;
u8 *func_801295C0(Obj801295C0 *obj, u8 *cursor) {
    obj->unkB7 = 1;
    return cursor;
}
