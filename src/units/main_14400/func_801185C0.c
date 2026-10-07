#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[8]; void *field_8; } Obj801185C0;
extern u8 D_8015DFA0[];
Obj801185C0 *func_80116D50(Obj801185C0 *obj, s32 arg1);

Obj801185C0 *func_801185C0(Obj801185C0 *obj) {
    func_80116D50(obj, 0xD);
    obj->field_8 = D_8015DFA0;
    return obj;
}
