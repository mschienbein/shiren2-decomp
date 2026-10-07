#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct { short delta; short pad; void (*fn)(void *, s32); } DestructorEntry;
typedef struct { short delta; short pad; void (*fn)(void *, u8); } KindEntry;
typedef struct {
    char pad0[8];
    DestructorEntry destroy;
    char pad10[0x40];
    KindEntry set_kind;
} VTable;
typedef struct { char pad0; u8 unk1; char pad2[6]; VTable *vtable; } Obj;
void func_800D3650(Obj *);
s32 func_8010B9F4(Obj *);
void *func_800AAEB4(s8 index);
/* Container slot +0x44 supplies self and actor; this override ignores both. */
void *func_80120A50(void *arg0, void *arg1, void *item) {
    Obj *obj = item;
    func_800D3650(obj);
    if (obj->unk1 == 0x6F && (short)func_8010B9F4(obj) >= 0x63) {
        obj->vtable->set_kind.fn((char *)obj + obj->vtable->set_kind.delta, 0x70);
        return obj;
    }
    if (obj != 0) {
        obj->vtable->destroy.fn((char *)obj + obj->vtable->destroy.delta, 3);
    }
    return func_800AAEB4(-1);
}
