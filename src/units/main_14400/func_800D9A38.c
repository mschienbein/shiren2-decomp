#include "common.h"

typedef unsigned char u8;

typedef struct {
    void *field0;
    void *field4;
} Elem;

/* Embedded member object destroyed by func_800D03C4 (vtable at +4, field at +0x10). */
typedef struct {
    u8 pad0[0x4];
    const void *vtable4;
    u8 pad8[0x8];
    s32 field10;
} Member;

typedef struct {
    s32 field0;
    const void *vtable4;
    Elem elems[21];
    Member memberB0;
} Obj_800D9A38;

extern const unsigned char D_80158098[48];
extern char D_80157FA8[];

void func_800D03C4(u8 *a, s32 f);
void func_800D8FE8(void *object);

/* Element destructor: its supplied element pointer is unused; it inlines to nothing. */
static inline void elem_destroy(Elem *elem) {
}

/* Destructor: tear down members in reverse order, restore the base vtable, free on flag bit 0. */
void func_800D9A38(Obj_800D9A38 *obj, s32 flags) {
    Elem *begin;

    obj->vtable4 = D_80158098;
    func_800D03C4((u8 *)&obj->memberB0, 2);
    begin = obj->elems;
    if (begin != 0) {
        Elem *elem = &obj->elems[21];

        while (begin != elem) {
            elem--;
            elem_destroy(elem);
        }
    }
    obj->vtable4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
