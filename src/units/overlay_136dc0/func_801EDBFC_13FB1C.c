#include "func_801EDBFC_13FB1C/types.h"

extern D_801F51EC_s *D_801F51EC_14710C;
extern u8 D_801F5CCD_147BED;
void func_801E5558_137478(s32);
s32 func_800D818C(s32);
s32 func_800D8144(s32);
s32 func_801E8D0C_13AC2C(u16, u16);
void *func_800A8CB0(s32 cell);
s32 func_800A8974(void *p);
void func_801EB6A0_13D5C0(u8);
void func_801E57E4_137704(s32);
void func_801E8F20_13AE40(s32);
void func_801E6114_138034(s32, s32);
void func_801E7E20_139D40(s32);
u8 func_801E53A4_1372C4(s32);
void func_801E7FD0_139EF0(s32, s32, s32, s32, u8);

s32 func_801EDBFC_13FB1C(void) {
    switch (D_801F5CCD_147BED) {
    case 0x37:
        D_801F51E8_147108->b000 = 1;
        D_801F51E8_147108->b001 = 0xFF;
        func_801E5558_137478(-1);
        D_801F51E8_147108->h004 = 0;
        D_801F51E8_147108->h006 = 0;
        D_801F51E8_147108->h008 = 0;
        D_801F51E8_147108->h010 = D_801F51E8_147108->x108;
        D_801F51E8_147108->h012 = D_801F51E8_147108->x10C;
        break;
    case 0x30: {
        Vec2i *cell;
        Vec2i pos;

        if (D_801F51E8_147108->x108 != 0x29) {
            D_801F51E8_147108->b001 = (u8)func_801E8D0C_13AC2C(D_801F51E8_147108->x108, D_801F51E8_147108->x10C);
        } else {
            D_801F51E8_147108->b001 = (u8)func_801E8D0C_13AC2C(0x29, func_800D8144(func_800D818C(D_801F51E8_147108->x10C)));
        }
        D_801F51E8_147108->b000 = 1;
        func_801E5558_137478(D_801F51E8_147108->b001);
        cell = func_800A8CB0(D_801F51E8_147108->b001);
        if (func_800A8974(cell)) {
            return 1;
        }
        pos = *cell;
        D_801F51E8_147108->h010 = pos.y;
        D_801F51E8_147108->h012 = pos.x;
        D_801F51E8_147108->h004 = 0;
        D_801F51E8_147108->h006 = 0;
        D_801F51E8_147108->h008 = 0;
        break;
    }
    case 0x31:
        D_801F51E8_147108->b000 = 1;
        func_801EB6A0_13D5C0(0);
        func_801EB6A0_13D5C0(1);
        func_801EB6A0_13D5C0(2);
        D_801F51E8_147108->h004 = D_801F51E8_147108->x108;
        D_801F51E8_147108->h006 = D_801F51E8_147108->x10C;
        D_801F51E8_147108->h008 = D_801F51E8_147108->x110;
        break;
    case 0x32:
        D_801F51E8_147108->b000 = 1;
        D_801F51E8_147108->h00C = D_801F51E8_147108->x108;
        break;
    case 0x33:
        D_801F51E8_147108->b000 = 1;
        D_801F51E8_147108->h00E = D_801F51E8_147108->x108;
        break;
    case 0x34:
        D_801F51E8_147108->b000 = 1;
        D_801F51E8_147108->h014 = D_801F51E8_147108->x108;
        break;
    case 0x35:
        func_801E57E4_137704(D_801F51E8_147108->b002);
        if (D_801F51E8_147108->b002 != 0) {
            func_801E8F20_13AE40(D_801F51E8_147108->b002);
        }
        return 1;
    case 0x36:
        func_801E6114_138034(D_801F51E8_147108->x108, D_801F51E8_147108->x10C);
        return 1;
    case 0x38:
        func_801E7E20_139D40(D_801F51E8_147108->x108);
        return 1;
    case 0x39:
        D_801F51E8_147108->b000 = 1;
        D_801F51E8_147108->h00A = D_801F51E8_147108->x108;
        break;
    case 0x3A:
        func_801E7FD0_139EF0(D_801F51E8_147108->x108, D_801F51E8_147108->x10C, D_801F51E8_147108->x110, D_801F51E8_147108->x114,
                      func_801E53A4_1372C4(D_801F51EC_14710C->x4++));
        return 1;
    default:
        return 0;
    }
    return 1;
}
