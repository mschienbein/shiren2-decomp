#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[4]; s16 field_4; u8 pad6[8]; s16 field_E; u8 pad10[6]; s16 field_16; } Obj8008AFCC;
void func_80051D14(s16 arg0);

void func_8008AFCC(Obj8008AFCC *obj) {
    func_80051D14(obj->field_16);
    obj->field_E = 1;
    obj->field_4 = 4;
}
