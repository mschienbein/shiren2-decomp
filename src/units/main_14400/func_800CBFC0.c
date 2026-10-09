#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.8.1 vtable entry: this-adjust delta, index, function. */
typedef struct {
    s16 delta;
    s16 index;
    void *func;
} VTableEntry;

typedef struct {
    u8 pad0[0x18];
    VTableEntry *vtable18;
} Obj800CBFC0;

extern Obj800CBFC0 *D_80147F44;
extern void func_800CBEBC(unsigned char, unsigned char);
extern s32 func_800CBE18(s32 id);
/* Disabled printf-style diagnostic. */
extern void func_80136964(const char *, ...);

void func_800CBFC0(void *arg0, u8 arg1) {
    s32 missing;

    func_800CBEBC(arg1, 0);
    missing = func_800CBE18(0x730) != 1;
    if (missing) {
        func_80136964("Ranking::_freadRawDungeon() cannot read\n");
    } else {
        Obj800CBFC0 *target = D_80147F44;
        VTableEntry *entry = &target->vtable18[5];

        ((void (*)(void *, s32, void *))entry->func)((u8 *)target + entry->delta, 0x730, arg0);
    }
}
