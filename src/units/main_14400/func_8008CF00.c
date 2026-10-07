#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 data[0x20]; } EntryA;
typedef struct { u8 data[0x78]; } EntryB;
typedef struct { EntryA a[8]; EntryB b[8]; s32 field_4C0; s32 field_4C4; } Obj;
void func_8008C6B0(EntryA *entry);
void func_8008C950(EntryB *entry);
void func_8008CF00(Obj *obj) {
    s32 i;

    obj->field_4C0 = 0;
    obj->field_4C4 = 0;
    for (i = 0; i < 8; i++) {
        func_8008C6B0(&obj->a[i]);
    }
    for (i = 0; i < 8; i++) {
        func_8008C950(&obj->b[i]);
    }
}
