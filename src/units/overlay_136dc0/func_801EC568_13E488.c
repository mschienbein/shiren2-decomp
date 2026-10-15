#include "func_801EC568_13E488/types.h"

extern s32 D_801F5D04_147C24;
void func_801EC510_13E430(void);
s32 func_801E5010_136F30(void);
void func_801EC4D0_13E3F0(void);
void func_80045A24(s32);
/* Host-contract owner: the integer slot is unused; the first unnamed argument
 * is text for fixed "%s". Provider migration and Opus review remain required. */
void func_80048334(s32 mode, s32 unused_message, ...);
void func_80048294(s32, u16, ...);
void func_800482E4(s32, u16, ...);
void func_800483FC(u16, ...);

s32 func_801EC568_13E488(void) {
    s32 flag = 0;
    /* ROM 0x13E4A8 / VRAM 0x801EC588 initializes a1 from the zero in a0. */
    s32 result = 0;

    if (D_801F51E8_147108->b9F4 != 0) {
        u8 c = D_801F51E8_147108->x2A6[D_801F51E8_147108->b9F4 - 1];

        if (c == 0x21 || c == 0x22) {
            func_801EC510_13E430();
            return 0;
        }
    }
    if (D_801F51E8_147108->len_11E != 0) {
        if (D_801F51E8_147108->b9F3 != 0) {
            func_801E5010_136F30();
            /* ROM 0x13E500 / VRAM 0x801EC5E0 writes a1 = 0 again after
             * the call, in the jump delay slot. The callee uses a1 at
             * 0x801E5088..0x801E50AC; this caller-saved register is not
             * preserved. Both zero writes execute on this branch. */
            result = 0;
        } else {
            switch (D_801F51E8_147108->b990) {
            case 1:
            case 5:
            case 6:
                D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E] = 0;
                if (D_801F51E8_147108->b99D == 1) {
                    D_801F51E8_147108->b99D = 0;
                    flag = 1;
                }
                if (D_801F5D04_147C24 == 1) {
                    func_80048334(flag, 0x4F9, D_801F51E8_147108->buf_126);
                }
                if (D_801F51E8_147108->b998 == 1 || D_801F51E8_147108->b999 != 2) {
                    if (D_801F51E8_147108->b997 != 0) {
                        if (D_801F51E8_147108->b997 != 3) {
                            func_801EC4D0_13E3F0();
                        } else {
                            func_80045A24(0x1A4);
                        }
                    }
                } else {
                    func_801EC510_13E430();
                }
                D_801F51E8_147108->b998 = 0;
                result = 1;
                break;
            case 2:
                if (D_801F51E8_147108->b99D == 1) {
                    D_801F51E8_147108->b99D = 0;
                    flag = 1;
                }
                D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E] = 0;
                if (D_801F5D04_147C24 == 1) {
                    func_80048294(flag, 0x4F9, D_801F51E8_147108->buf_126);
                }
                result = 1;
                break;
            case 3:
                if (D_801F51E8_147108->b99D == 1) {
                    D_801F51E8_147108->b99D = 0;
                    flag = 1;
                }
                D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E] = 0;
                if (D_801F5D04_147C24 == 1) {
                    func_800482E4(flag, 0x4F9, D_801F51E8_147108->buf_126);
                }
                result = 1;
                break;
            case 4:
                D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E] = 0;
                if (D_801F5D04_147C24 != 0) {
                    func_800483FC(0x4F9, D_801F51E8_147108->buf_126);
                }
                result = 1;
                break;
            }
        }
        D_801F51E8_147108->len_11E = 0;
    }
    return result;
}
