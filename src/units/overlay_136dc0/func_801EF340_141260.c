#include "func_801EF340_141260/types.h"
extern S10EventState *D_801F51E8_147108;
extern u8 D_801F5D0C_147C2C;
extern u16 D_801F5D0E_147C2E;
extern s32 func_80049CB4(s32 event, ...);
extern void func_801F21E4_144104(void);
extern void func_801F33FC_14531C(void);
extern void func_801EEC9C_140BBC(u16 id, s32 mode);
extern void func_801EEE14_140D34(s32 reason);
extern s32 func_801EF140_141060(s32 mode);

/* ODD_C: the byte predicate retains the ROM's andi/xori boolean negation.
 * Direct !(D_801F5D0C_147C2C & 1) removes xori, emitting 372 instead of 376 bytes. */
static inline s32 event_inactive(void)
{
    u8 active = D_801F5D0C_147C2C & 1;
    return active ^ 1;
}

s32 func_801EF340_141260(s32 mode, s32 flag)
{
    u8 result;
    func_801F21E4_144104();
    if (D_801F5D0E_147C2E == 0) {
        return 0;
    }
    func_80049CB4(14);
    if (event_inactive()) {
        func_801EEC9C_140BBC(D_801F5D0E_147C2E, mode);
        D_801F51E8_147108->field9F8 = flag;
        D_801F5D0C_147C2C |= 1;
        if ((D_801F5D0C_147C2C >> 1) & 1) {
            D_801F51E8_147108->field990 = 6;
        }
    }
    func_801F33FC_14531C();
    result = func_801EF140_141060((D_801F5D0C_147C2C >> 1) & 1);
    switch (result) {
    case 0:
    case 8:
    case 10:
    case 11:
    case 12:
        if (D_801F51E8_147108->field9FC) {
            result = 13;
        }
        func_801EEE14_140D34(result);
        break;
    }
    func_80049CB4(13);
    if (result == 12) {
        return 5;
    } else if (result == 10) {
        return 13;
    } else if (result == 8) {
        return 10;
    } else if (result == 11) {
        return 6;
    } else if (result == 13) {
        return 15;
    }
    return 0;
}
