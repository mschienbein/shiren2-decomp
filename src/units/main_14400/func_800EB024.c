#include "common.h"
typedef struct { unsigned char pad[4]; unsigned char field4; } Result;
typedef struct { unsigned char pad[0xA0]; short fieldA0; Result *(*fieldA4)(void *self, unsigned char index); } Methods;
typedef struct { unsigned char pad[0x24]; Methods *field24; } Object;
extern s32 func_800E0F40(Object *);
unsigned char func_800EB024(Object *p) {
    s32 value = func_800E0F40(p) & 0xFF;
    Methods *methods = p->field24;
    return methods->fieldA4((unsigned char *)p + methods->fieldA0, value)->field4;
}
