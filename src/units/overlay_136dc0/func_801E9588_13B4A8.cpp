#include "common.h"
typedef unsigned char u8;
typedef short s16;

#include "ovlb_shared_views.h"
extern "C" {
extern const u8 D_8015488C[8];
Game800C9E00 *func_800C9E00(void);
OvlBItemDispatchView *func_800AC244(u8 id);
void func_801EA810_13C730(OvlBItemDispatchView *item, s32 value);
s32 func_800AD714(OvlBItemDispatchView *item, MenuPos *pos);
void func_800AD7E0(OvlBItemDispatchView *item, MenuPos *pos, s32 notify);
s32 func_80049CB4(s32 id, ...);
void func_800D1BB0(Game800C9E00 *game);
}

static inline s32 test_bit(const u8 *bits, s32 bit) {
    return bits[bit >> 3] & D_8015488C[bit & 7];
}



static inline u8 cell_flags(Game800C9E00 *game, s32 pick) {
    s32 index = pick - 1;
    s32 row = index / 4;
    s32 column = index - row * 4;
    u8 *entries = game->extra[row];
    return entries[column];
}

extern "C" void func_801E9588_13B4A8(u8 *picks) {
    s32 i = 4;
    Game800C9E00 *game = func_800C9E00();
    while (i--) {
        MenuPos pos;
        u8 bits;
        s32 j;
        if (picks[i] == 0) {
            continue;
        }
        bits = cell_flags(game, picks[i]);
        j = 5;
        while (j--) {
            OvlBItemDispatchView *item;
            if (!test_bit(&bits, j)) {
                continue;
            }
            item = func_800AC244(j - 23);
            if (item == 0) {
                continue;
            }
            func_801EA810_13C730(item, 3);
            switch (j) {
            case 4:
                pos.y = 0x1F;
                pos.x = 0x16;
                break;
            case 3:
                pos.y = 0x20;
                pos.x = 0x17;
                break;
            case 2:
                pos.y = 0x1E;
                pos.x = 0x17;
                break;
            case 1:
                pos.y = 0x1D;
                pos.x = 0x16;
                break;
            case 0:
                pos.y = 0x1C;
                pos.x = 0x17;
                break;
            }
            if (func_800AD714(item, &pos)) {
                func_800AD7E0(item, &pos, 1);
            } else if (item) {
                item->table_08->destroy_0C((u8 *)item + item->table_08->delta_08, 3);
            }
        }
    }
    func_80049CB4(2);
    func_800D1BB0(game);
}
