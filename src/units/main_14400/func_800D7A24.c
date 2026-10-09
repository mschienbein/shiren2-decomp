#include "common.h"
typedef unsigned char u8;
extern unsigned char D_80148190[];
extern unsigned char D_801480DC[];
extern const unsigned char D_8015488C[8];
extern s32 func_800D7D84(u8 kind, u8 level);
extern s32 func_800D7860(s32 index);
extern void *func_800D7B04(u8 kind, u8 level);
static inline void mark_index(s32 index) {
    unsigned char *bits = &D_801480DC[index >> 3];
    *bits |= D_8015488C[index & 7];
}
void *func_800D7A24(unsigned char x, unsigned char y) {
    s32 index = func_800D7D84(x, y);
    if (index == -1) return 0;
    if (!D_80148190[index]) return 0;
    {
        s32 available = func_800D7860(index) != 1;
        if (available) {
            void *result = func_800D7B04(x, y);
            if (result) {
                mark_index(index);
                D_80148190[index] = (D_80148190[index] + 1) >> 1;
                return result;
            }
        }
    }
    return 0;
}
