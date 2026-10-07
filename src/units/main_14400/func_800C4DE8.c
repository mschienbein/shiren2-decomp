#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x10]; u16 *field_10; } Obj800C4DE8;
char *func_800C4EE0(u16 *arg);
char *func_800C4DE8(Obj800C4DE8 *obj) {
    return func_800C4EE0(obj->field_10);
}
