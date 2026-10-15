#include "common.h"

typedef unsigned short u16;
struct Obj { unsigned char kind, id; };

extern "C" {
extern char D_80147620[];
/*
 * Read-only item parameter halfwords (lower, upper) for item ids 0x76..0x7A,
 * declared individually like the neighbouring 0x801569BA..0x801569C0 ranges
 * used by func_800AB6B8 and func_800AB704.
 */
extern const u16 D_801569A0;
extern const u16 D_801569A2;
extern const u16 D_801569A4;
extern const u16 D_801569A6;
extern const u16 D_801569A8;
extern const u16 D_801569AA;
extern const u16 D_801569AC;
extern const u16 D_801569AE;
extern const u16 D_801569B0;
extern const u16 D_801569B2;
s32 func_800C5954(void *, u16, u16);
void func_800AE6C4(Obj *, u16);

void func_800AB5B8(Obj *item)
{
    /*
     * A g++ function-local static with dynamic initializers, as in
     * func_800EA3F0: filled on first use under its guard word (the .data
     * word at 0x80142F30; the table is the 20-byte .bss array at 0x801C5210).
     */
    static u16 ranges[5][2] = {
        { D_801569A0, D_801569A2 },
        { D_801569A4, D_801569A6 },
        { D_801569A8, D_801569AA },
        { D_801569AC, D_801569AE },
        { D_801569B0, D_801569B2 }
    };
    u16 *range = ranges[item->id - 0x76];
    func_800AE6C4(item, func_800C5954(D_80147620, range[0], range[1]));
}
}
