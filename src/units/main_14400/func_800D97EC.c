#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Record { void *field_0; void *field_4; } Record;
typedef struct Obj {
    s32 field_00;
    const VTable *vtable_04;
    Record records_08[21];
    /* Complete list member: func_800D03C4 clears the word at member +0x10. */
    u8 sub_B0[0x14];
} Obj;
extern const VTable D_80158068, D_80157FA8;
extern void func_800D03C4(u8 *a, s32 f);
extern void func_800D8FE8(void *object);
void func_800D97EC(Obj *self, s32 flags) {
    Record *begin;
    Record *end;
    self->vtable_04 = &D_80158068;
    func_800D03C4(self->sub_B0, 2);
    begin = self->records_08;
    if (begin) {
        end = self->records_08 + 21;
        while (begin != end) --end;
    }
    self->vtable_04 = &D_80157FA8;
    if (flags & 1) func_800D8FE8(self);
}
