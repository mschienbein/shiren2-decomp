#include "common.h"

typedef unsigned short u16;

extern u16 D_80143450[][76];
void func_800B68DC(void *room, s32 index);

void func_800B2118(s32 *pos, void *room, s32 index) {
    s32 outside;

    outside = 0;
    if (pos[1] >= 76 || pos[0] >= 54 || pos[1] < 0 || pos[0] < 0) {
        outside = 1;
    }
    if (outside) {
        return;
    }
    D_80143450[pos[0]][pos[1]] |= 0x800;
    if (room != 0) {
        func_800B68DC(room, index);
    }
}
