#include "common.h"

typedef struct {
    char pad0[0x20];
    short delta_20;
    short index_22;
    s32 (*func_24)(void *self);
} VTable80115514;

typedef struct {
    s32 field_0;
    VTable80115514 *vtable_4;
} Sub80115514;

typedef struct {
    char pad0[0xC];
    Sub80115514 sub_C;
} Obj80115514;

void func_80115514(Obj80115514 *obj) {
    Sub80115514 *sub = &obj->sub_C;
    VTable80115514 *vtable = sub->vtable_4;

    (void)vtable->func_24((char *)sub + vtable->delta_20);
}
