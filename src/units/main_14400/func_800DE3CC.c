#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; s32 unk4; } Elem;
/* func_800D03C4 restores vtable_4 and clears field_10. */
typedef struct { s32 field_0; void *vtable_4; u8 unknown_8[8]; s32 field_10; } Member;
typedef struct {
    s32 unk0;
    void *vtable;
    Elem elems[21];
    Member unkB0;
} S;
extern u8 D_80157FA8[];
void func_800D03C4(Member *, s32);
void func_800D8FE8(void *);
void func_800DE3CC(S *self, s32 flags) {
    func_800D03C4(&self->unkB0, 2);
    if (self->elems != 0) {
        Elem *p = &self->elems[21];
        while (self->elems != p) {
            p--;
        }
    }
    self->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
