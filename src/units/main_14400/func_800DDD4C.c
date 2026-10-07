#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 unk0; s32 unk4; } Elem800DDD4C;
/* The member destructor writes its vtable at +4 and clears its word at +0x10. */
typedef struct { s32 unk0; void *vtable; s32 unk8; s32 unkC; s32 unk10; } Member800DDD4C;
typedef struct { s32 unk0; void *vtable; Elem800DDD4C elems[21]; Member800DDD4C member; } Obj800DDD4C;
extern u8 D_80157FA8[];
void func_800D03C4(void *member, s32 flags);
void func_800D8FE8(void *ptr);
void func_800DDD4C(Obj800DDD4C *obj, s32 flags) {
    Elem800DDD4C *end = obj->elems + 21;
    Elem800DDD4C *begin;
    func_800D03C4(&obj->member, 2);
    begin = obj->elems;
    if (begin != 0) {
        Elem800DDD4C *p = end;
        while (begin != p) {
            p--;
        }
    }
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
