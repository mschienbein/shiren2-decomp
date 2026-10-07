#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xBC];
    u8 valueBC;
} Obj80129A34;

u8 *func_80129A34(Obj80129A34 *obj, u8 *cursor) {
    obj->valueBC = *cursor;
    return cursor + 1;
}
