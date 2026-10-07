#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct VEntry { s16 delta; s16 index; void (*func)(void *, s32, u8 *); } VEntry;
typedef struct { u8 pad[0x18]; VEntry *vtable; } S;
void func_800CA4A4(S *, char *);
extern char D_80153694[];
extern u8 D_80142F24[11];
extern u8 D_80142F20[];
/* These linker-defined globals are separate objects in the N64's 32-bit
 * address space. Use integer addresses, not an out-of-bounds array index,
 * for the bytes at 0x80142F20..23 and 0x80142F1B. */
#define VCALL(self, size, ptr) (self)->vtable[3].func((u8 *)(self) + (self)->vtable[3].delta, size, ptr)
void func_800A9520(S *self) {
    func_800CA4A4(self, D_80153694);
    VCALL(self, 11, D_80142F24);
    VCALL(self, 1, D_80142F20);
    VCALL(self, 1, (u8 *)((u32)D_80142F20 + 1));
    VCALL(self, 1, (u8 *)((u32)D_80142F20 + 2));
    VCALL(self, 1, (u8 *)((u32)D_80142F20 - 5));
    VCALL(self, 1, (u8 *)((u32)D_80142F20 + 3));
}
