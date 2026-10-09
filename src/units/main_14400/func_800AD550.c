#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
struct Rng;
extern struct Rng D_80147620;
extern const u8 D_80153734[];
extern const u16 *D_80153764[];
extern u16 *D_801537B8[];
s32 func_800C5844(void *rng, u8 base, u8 top);
/* ODD_C: The accessor retains the original second table-load scheduling. */
static inline s32 length(u8 index) { return D_80153734[index]; }
void func_800AD550(s32 value) {
    s32 index;
    u16 *cursor;
    const u16 *source = D_80153764[(u8)value];
    u16 *destination = D_801537B8[(u8)value];
    if (source) {
        index = D_80153734[(u8)value];
        while (--index != -1)
            destination[index] = source[index];
        index = length(value);
        /* local-arithmetic-qualification: In-bounds destination + index, index +
         * destination, and split += forms all reverse the original addu operands. */
        cursor = (u16 *)((u32)(index * sizeof(*cursor)) + (u32)destination);
        for (;;) {
            s32 other;
            if (--index == -1) break;
            --cursor;
            other = (u8)func_800C5844(&D_80147620, 0, index);
            if (index != other) {
                u16 saved = *cursor;
                *cursor = destination[other];
                destination[other] = saved;
            }
        }
    }
}
