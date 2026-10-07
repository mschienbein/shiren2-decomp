#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    void *field_0;
    s32 field_4;
    s32 field_8;
    u8 field_C;
    void *field_10;
} Params80130F90;

typedef struct {
    u8 pad0[8];
    s32 handle_8;
} Obj80130F90;

s32 func_80131F04(s32 kind, Params80130F90 *params);

void func_80130F90(Obj80130F90 *obj, s32 a, s32 b, void *buffer, s32 c) {
    Params80130F90 params;

    params.field_4 = a;
    params.field_0 = obj;
    params.field_8 = b;
    params.field_10 = buffer;
    params.field_C = c;
    obj->handle_8 = func_80131F04(0x204, &params);
}
