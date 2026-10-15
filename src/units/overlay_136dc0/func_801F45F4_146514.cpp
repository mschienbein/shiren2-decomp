#include "func_801F45F4_146514/types.h"

extern "C" {
extern u8 D_801F5CCD_147BED;
extern s32 D_801F5CD0_147BF0;
extern Obj801F51EC *D_801F51EC_14710C;
extern s8 D_80140160[];
u8 func_801E53A4_1372C4(s32 index);
void func_801EB6A0_13D5C0(u8 arg0);
void func_801E64D0_1383F0(s32 arg0, s32 top, s32 arg2, s32 arg3, s32 arg4);
void func_801E7268_139188(s32 slot, s32 x, s32 z);
void func_800AABEC(void);
void func_80055B14(u32 arg0, u32 kind);
void func_801E7934_139854(void);
void func_801F3664_145584(void);
void func_801F3B5C_145A7C(void);
}

static inline s32 isDisplayModeOne(const s8 *menu) { return menu[4] == 1; }
static inline s32 menu_mode(const s8 *menu) { return menu[6]; }

extern "C" void func_801F45F4_146514(void) {
    s32 i;

    switch (D_801F5CCD_147BED) {
    case 100: {
        s32 top = func_801E53A4_1372C4(D_801F51EC_14710C->unk04++);

        func_801EB6A0_13D5C0(0);
        func_801E64D0_1383F0(D_801F51E8_147108->args[0].word, top, D_801F51E8_147108->args[1].word, D_801F51E8_147108->args[2].word, D_801F51E8_147108->args[3].word);
        for (i = 30;;) {
            OvlBUnitHead *unit;

            i--;
            if (i == -1) {
                break;
            }
            unit = func_800A8CB0((u8)i);
            if (func_800A8974(unit)) {
                continue;
            }
            OvlBPos pos = unit->pos;
            if (pos.y < top) {
                continue;
            }
            if (D_801F51E8_147108->args[2].word < pos.y) {
                continue;
            }
            if (pos.x < D_801F51E8_147108->args[1].word) {
                continue;
            }
            if (D_801F51E8_147108->args[3].word < pos.x) {
                continue;
            }
            func_801E7268_139188(i, pos.y, pos.x);
        }
        break;
    }
    case 127:
        func_800AABEC();
        break;
    case 126:
        func_80055B14(D_801F51E8_147108->args[0].word, D_80142F18.kind);
        if (menu_mode(D_80140160) == 1) {
            func_801E7934_139854();
        }
        if (D_80140160[6] == 2 || !isDisplayModeOne(D_80140160)) {
            func_801E7934_139854();
        }
        break;
    case 124:
        D_801F5CD0_147BF0 = 1;
        break;
    case 101:
    case 102:
    case 115:
    case 116:
    case 117:
        func_801F3664_145584();
        break;
    default: {
        s32 count;

        if (D_801F51E8_147108->args[0].word == 0 || (u8)func_801E8D0C_13AC2C((u16)D_801F51E8_147108->args[0].word, 0) != 0) {
            func_801F3B5C_145A7C();
            break;
        }
        count = 0;
        switch (D_801F5CCD_147BED) {
        case 108:
            count = 4;
            break;
        case 109:
        case 114:
        case 120:
            count = 1;
            break;
        case 123:
            count = 5;
            break;
        case 119:
            count = 6;
            break;
        }
        while (count--) {
            func_801E53A4_1372C4(D_801F51EC_14710C->unk04++);
        }
        break;
    }
    }
}
