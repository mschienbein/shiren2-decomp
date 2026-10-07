#include "common.h"

typedef short s16;
typedef unsigned char u8;
typedef struct {
    u8 field_00[0x80];
    s16 field_80;
    s16 field_82;
    void (*field_84)(void *, void *);
} Methods;
typedef struct {
    u8 field_00[0x3C];
    u8 field_3C[0x10];
    Methods *field_4C;
} Object;

void func_800960A0(Object *object) {
    object->field_4C->field_84((u8 *)object + object->field_4C->field_80, object->field_3C);
}
