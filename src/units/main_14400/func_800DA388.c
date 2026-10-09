#include "common.h"

typedef unsigned short u16;
typedef struct {
    u16 field_00;
    u16 field_02;
    void *field_04;
} Object;
extern s32 D_80157FA8[];
extern s32 D_80158278[];

/* The caller supplies a payload, but this fixed-kind constructor does not read it. */
Object *func_800DA388(Object *object, unsigned char *unused_payload) {
    /* Base-command initialization (func_800D906C), then the derived vtable. */
    object->field_04 = D_80157FA8;
    object->field_00 = 0x38;
    object->field_04 = D_80158278;
    return object;
}
