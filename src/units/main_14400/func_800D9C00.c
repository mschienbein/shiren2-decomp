#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 data[0x18]; } Entry18;
typedef struct { s16 x0; s16 pad2; void *x4; void *x8; } Obj;
extern u8 D_80157FA8[];
extern u8 D_801580C8[];
extern Entry18 D_80143330[];
Obj *func_800D9C00(Obj *obj, u8 *arg) {
    Entry18 *table;
    obj->x4 = D_80157FA8;
    obj->x0 = 10;
    obj->x4 = D_801580C8;
    table = D_80143330;
    obj->x8 = &table[*arg];
    return obj;
}
