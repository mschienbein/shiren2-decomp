#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern u8 D_8015B748[];
void func_800EFD28(void *self, s32 flags);
void func_800A3918(void *ptr);

typedef struct {
    u8 pad0[0x24];
    void *vtable;
} Obj80101F58;

void func_80101F58(Obj80101F58 *self, s32 flags) {
    self->vtable = D_8015B748;
    func_800EFD28(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
