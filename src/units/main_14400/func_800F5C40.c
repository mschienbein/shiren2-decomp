#include "common.h"
typedef struct { unsigned char pad[0xA]; unsigned char fieldA; unsigned char padB[0x14]; unsigned char field1F; s32 field20; const void *field24; s32 field28; unsigned char field2C; } Object;
extern Object *func_800A38FC(s32);
extern void *func_800F4760(Object *);
extern const unsigned char D_80159938[120];
Object *func_800F5C40(void) {
    Object *p = func_800A38FC(0x30);
    func_800F4760(p);
    p->field24 = D_80159938;
    p->fieldA = 0x13;
    p->field1F = 0x13;
    p->field2C = 0;
    return p;
}
