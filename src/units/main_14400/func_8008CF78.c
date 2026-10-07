#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 data[0x20]; } SmallSlot8008CF78;
typedef struct { u8 data[0x78]; } BigSlot8008CF78;
typedef struct {
    SmallSlot8008CF78 small[8];
    BigSlot8008CF78 big[8];
    s32 field_4C0;
    s32 field_4C4;
} Obj8008CF78;
void func_8008C9F4(BigSlot8008CF78 *slot);
void func_8008C75C(SmallSlot8008CF78 *slot);

void func_8008CF78(Obj8008CF78 *obj) {
    s32 i;

    obj->field_4C0 = 0;
    obj->field_4C4 = 0;
    for (i = 0; i < 8; i++) {
        func_8008C9F4(&obj->big[i]);
    }
    for (i = 0; i < 8; i++) {
        func_8008C75C(&obj->small[i]);
    }
}
