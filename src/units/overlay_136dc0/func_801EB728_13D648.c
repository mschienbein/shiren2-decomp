#include "func_801EB728_13D648/types.h"
extern void func_801EB6A0_13D5C0(u8 index);
extern void func_80041434(s32 value);
/* ROM 0x13D734 passes a full word; the host facade narrows it locally
 * at ROM 0x18CE4/0x18CF0 before calling func_800522FC. */
extern void func_80045CA0(s32 value);
extern void func_801E8634_13A554(s32 x, s32 y, s32 z);
extern void func_801E8758_13A678(s32 a, s32 b, s32 c);
extern void func_801E8AB8_13A9D8(s32 a, s32 enabled);
extern void func_801E817C_13A09C(u32 a, s32 b, float c);
extern void func_801E830C_13A22C(u16 *values, u8 mode);

s32 func_801EB728_13D648(void)
{
    switch (D_801F5CCD_147BED) {
    case 0x5F:
        if (!D_801F51E8_147108->field108[1].word) {
            D_801F51E8_147108->field120 = D_801F51E8_147108->field108[0].half[1];
        }
        if (D_801F51E8_147108->field108[2].word) {
            D_801F51E8_147108->field120 |= 0x8000;
        }
        D_801F51E8_147108->field9F5 = D_801F5CCC_147BEC;
        break;
    case 0x53:
        func_80041434(D_801F51E8_147108->field108[0].word != 0);
        break;
    case 0x5A:
        func_801EB6A0_13D5C0(1);
        func_801EB6A0_13D5C0(2);
        func_801EB6A0_13D5C0(3);
        func_801E8634_13A554(D_801F51E8_147108->field108[1].word,
                     D_801F51E8_147108->field108[2].word,
                     D_801F51E8_147108->field108[3].word);
        break;
    case 0x52:
        func_80045CA0(D_801F51E8_147108->field108[0].word);
        break;
    case 0x57:
        if (func_801E53A4_1372C4(D_801F51EC_14710C->field4++) == 0) {
            func_801E8758_13A678(D_801F51E8_147108->field108[1].word,
                         D_801F51E8_147108->field108[2].word,
                         D_801F51E8_147108->field108[3].word);
        } else {
            func_801E8AB8_13A9D8(D_801F51E8_147108->field108[1].word,
                         D_801F51E8_147108->field108[2].word == 0);
        }
        break;
    case 0x5B:
        func_801EB6A0_13D5C0(2);
        func_801E817C_13A09C(D_801F51E8_147108->field108[0].word,
                     D_801F51E8_147108->field108[1].word,
                     (float)D_801F51E8_147108->field108[2].word);
        break;
    case 0x5E:
        {
            u16 values[4];
            s32 mode = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
            func_801EB6A0_13D5C0(0);
            func_801EB6A0_13D5C0(1);
            func_801EB6A0_13D5C0(2);
            func_801EB6A0_13D5C0(3);
            values[0] = D_801F51E8_147108->field108[0].half[1];
            values[1] = D_801F51E8_147108->field108[1].half[1];
            values[2] = D_801F51E8_147108->field108[2].half[1];
            values[3] = D_801F51E8_147108->field108[3].half[1];
            func_801E830C_13A22C(values, mode);
        }
        break;
    default:
        return 0;
    }
    return 1;
}
