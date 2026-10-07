#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 kind;
    u8 min;
    u8 max;
} Range;
extern u8 D_80156A45;
extern u8 D_80147620[];
extern Range D_80156D40[];
extern u8 D_80142F24[];
extern u8 D_80142F18;
s32 func_800C587C(void *, u8);
s32 func_800C5844(void *, u8, u8);
void *func_800AC244(u8);
static inline s32 isSelectable(void) {
    return func_800C587C(D_80147620, D_80156A45) == 1;
}
static inline s32 inRange(Range *range, u8 value) {
    return value >= range->min && value <= range->max;
}
u8 *func_800AB9C4(void) {
    u8 bits[24];
    s32 count;
    s32 i;
    u8 *obj;

    if (!isSelectable()) {
        return 0;
    }
    count = 0;
    i = 17;
    while (1) {
        Range *range;
        u8 *byte;
        s32 mask;

        i--;
        if (i == -1) {
            break;
        }
        range = &D_80156D40[i];
        byte = &bits[i / 8];
        mask = 1 << (i % 8);

        if (D_80142F24[0] == range->kind && inRange(range, D_80142F18)) {
            *byte |= mask;
            count++;
        } else {
            *byte &= ~mask;
        }
    }
    if (count == 0) {
        return 0;
    }
    if (count >= 2) {
        count = (u8)func_800C5844(D_80147620, 1, count);
    }
    i = 17;
    while (1) {
        i--;
        if (i == -1) {
            break;
        }
        if ((bits[i / 8] >> (i % 8)) & 1) {
            if (--count == 0) {
                obj = func_800AC244(0xEE);
                if (obj != 0) {
                    *(s32 *)(obj + 0xC) = i;
                }
                return obj;
            }
        }
    }
    return 0;
}
