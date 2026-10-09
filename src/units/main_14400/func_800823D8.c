#include "common.h"
#include "row_views.h"
extern u16 D_801A9058[10];
/* The low halfword is a value view of this word, not an overlapping object. */
extern s32 D_8013E818;
extern s32 D_8013E8E0;
extern void func_80081798(void);
static inline u32 get_size(u32 index) {
    return D_801A9080[index].field_02;
}
static inline s32 reverse_offset(s32 index) {
    return D_8013E818 - index;
}
static inline s32 last_position(void) {
    return (u16)D_8013E818 - 1;
}
void func_800823D8(s32 index, s32 distance) {
    s32 cursor;
    if ((u32)index < 10U) {
        u32 size = get_size(index);
        if (size != 0 && get_size(index) >= 64U) {
            if (distance >= D_8013E818) {
                distance = D_8013E818 - 1;
            }
            if (D_8013E818 - distance - 1 != D_801A9080[index].field_0E) {
                D_801A9080[index].field_0E = 0xFFFF;
                func_80081798();
                for (cursor = 0; cursor < distance; cursor++) {
                    s32 offset = reverse_offset(cursor + 2);
                    D_801A9080[D_801A9058[offset]].field_0E++;
                    D_801A9080[D_801A9058[offset]].field_10 = 1;
                }
                {
                    s32 last = last_position();
                    D_801A9080[index].field_10 = 1;
                    D_801A9080[index].field_0E = last - distance;
                }
                func_80081798();
                D_8013E8E0 = 1;
            }
        }
    }
}
