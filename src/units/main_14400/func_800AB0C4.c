#include "common.h"
typedef unsigned char u8;
/* 8-byte rows of the 8-row range table D_80156CD0 (see func_800AA24C, which returns
 * row i as i + 12); the spawn chance byte is at +6. */
typedef struct { u8 lo; u8 hi; char pad2[3]; u8 kind; u8 chance; char pad7; } Range;
extern Range D_80156CD0[8];
extern char D_80147620[];
extern s32 func_800AA24C(void);
extern s32 func_800C587C(void *, u8);
extern void *func_800AC244(u8);
void *func_800AB0C4(void) {
    s32 r = func_800AA24C();
    if ((u8)(r - 12) < 8) {
        /* Chance byte (+6) of row r - 12, read through a byte view of the real table.
         * The guard limits (u8)r to 12..19, so the byte offset is computed first and
         * stays within the 64-byte table (6..62); the in-bounds member form
         * D_80156CD0[(u8)r - 12].chance does not fold the row bias into the symbol
         * offset under gcc281 (no match, object 4 bytes over the slot). */
        if (func_800C587C(D_80147620, ((u8 *)D_80156CD0)[(u8)r * sizeof(Range) - 12 * sizeof(Range) + 6])) return func_800AC244(0xCF);
        return 0;
    }
    return 0;
}
