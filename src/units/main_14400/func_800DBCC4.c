#include "common.h"
typedef struct { unsigned char pad_00[4]; const void *vtable_04; } Obj800DBCC4;
extern const unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DBCC4(Obj800DBCC4 *self, s32 flags) {
    self->vtable_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
