#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Partial view: only the signed word at +0x10 is read here. */
typedef struct Obj8012A798 {
    u8 pad0[0x10];
    s32 field_10;
} Obj8012A798;

extern Obj8012A798 *D_801CA6F8;

Obj8012A798 *func_8012A798(Obj8012A798 *obj)
{
    if (obj != 0 && obj->field_10 < 0) {
        D_801CA6F8 = obj;
    }
    return D_801CA6F8;
}
