#include "common.h"
typedef unsigned char u8;
/* D_801480F4: 9-byte table (indexed 0..8 at 0x800D7F18). The 1-based threshold
 * index below stores element i - 1; the assembler folds the -1 into the
 * relocation, which splat labels D_801480F3. */
extern u8 D_80148190[], D_801480D0[], D_801480F4[9], D_801480DC[];
extern const u8 D_80154894[8];
extern s32 func_800D7D84(u8, u8);
extern s32 func_800D7860(s32);
extern void func_800D7C34(u8, u8);
static inline u8 *bit_byte(u8 *base, s32 index) { return base + (index >> 3); }
s32 func_800D78F4(u8 kind, u8 value, u8 level) {
    s32 index = func_800D7D84(kind, value);
    if (index == -1) return 0;
    if (!D_80148190[index] || func_800D7860(index)) {
        D_80148190[index] = level + 1;
        if (kind == 0x29) {
            s32 i;
            for (i = 1; i < 9; i++) {
                u32 limit = D_801480D0[i];
                if (value < limit) break;
            }
            D_801480F4[i - 1] = value;
        }
        {
            u8 *bits = bit_byte(D_801480DC, index);
            *bits &= D_80154894[index & 7];
        }
        func_800D7C34(kind, value);
        return 1;
    }
    return 0;
}
