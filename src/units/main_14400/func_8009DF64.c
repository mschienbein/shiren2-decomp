#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0x4C]; void *field_4C; } Obj8009DF64;
extern u8 D_80152C18[];
Obj8009DF64 *func_800953C0(Obj8009DF64 *obj);

Obj8009DF64 *func_8009DF64(Obj8009DF64 *obj) {
    func_800953C0(obj);
    obj->field_4C = D_80152C18;
    return obj;
}
