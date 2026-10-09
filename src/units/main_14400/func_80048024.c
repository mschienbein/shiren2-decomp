#include "common.h"

/* Complete 0x5C-byte menu (.bss 0x80160BC0..0x80160C1B); its vtable pointer is at +0x4C. */
typedef struct { unsigned char pad0[0x4C]; const void *field_4C; unsigned char pad_50[0xC]; } Obj;

extern Obj D_80160BC0;
/* Base menu vtable. */
extern const unsigned char D_80151E38[144];
void func_800D8FA8(void *object);

/* Base menu destructor (as func_800960E0): flags bit 0 frees the storage. */
static inline void menu_destroy(Obj *self, s32 flags)
{
    self->field_4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}

/* Static destructor of the D_80160BC0 menu: never frees the static storage. */
void func_80048024(void)
{
    menu_destroy(&D_80160BC0, 2);
}
