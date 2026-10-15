#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.8.1 vtable entry: this-adjust delta, index, function.
   Slot +0x18/+0x1C writes `size` bytes from `data`. */
typedef struct {
    s16 delta;
    s16 index;
    void (*fn)(void *self, s32 size, void *data);
} VEntry;

typedef struct {
    u8 pad0[0x18];
    VEntry *vtbl;
} Obj;

extern Obj *D_80147F44;
extern void func_800CBEBC(unsigned char, unsigned char);
extern s32 func_800CBE18(s32 amount);
/* Disabled printf-style diagnostic. */
extern void func_80136964(const char *, ...);
extern void func_800CA2C0(void *a);

void func_800CC0E8(void *data, unsigned char id) {
    s32 failed;

    func_800CBEBC(id, 0);
    failed = func_800CBE18(0x730) != 1;
    if (failed) {
        func_80136964("Ranking::_fwriteRawDungeon() cannot write\n");
    } else {
        D_80147F44->vtbl[3].fn((u8 *)D_80147F44 + D_80147F44->vtbl[3].delta, 0x730, data);
        func_800CA2C0(D_80147F44);
    }
}
