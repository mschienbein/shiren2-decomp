#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct VEntry { s16 delta; s16 index; void (*func)(void *, s32, u8 *); } VEntry;
typedef struct { u8 pad[0x18]; VEntry *vtable; } S;
void func_800CA4A4(S *, char *);
extern char D_80153694[];


/* Save the eleven-byte mutable state, then selected bytes of the loaded record. */
#define VCALL(self, size, ptr) (self)->vtable[3].func((u8 *)(self) + (self)->vtable[3].delta, size, ptr)
void func_800A9520(S *self) {
    func_800CA4A4(self, D_80153694);
    VCALL(self, 11, (u8 *)&D_80142F24);
    VCALL(self, 1, &D_80142F18.mode);
    VCALL(self, 1, (u8 *)&D_80142F18.coordinates[0]);
    VCALL(self, 1, (u8 *)&D_80142F18.coordinates[1]);
    VCALL(self, 1, &D_80142F18.flags);
    VCALL(self, 1, (u8 *)&D_80142F18.status);
}
