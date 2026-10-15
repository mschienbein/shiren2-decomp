#include "func_801EF140_141060/types.h"
extern S10EventState *D_801F51E8_147108;
extern S10ScriptFrame *D_801F51EC_14710C;
extern s8 D_801F5CCC_147BEC;
extern u8 D_801F5CCD_147BED;
extern s32 func_801EE9A8_1408C8(void);
extern u8 func_801E53A4_1372C4(s32 offset);
extern s32 func_801EBCA8_13DBC8(void);
extern s32 func_801ED008_13EF28(void);
extern s32 func_801ED720_13F640(void);
extern u16 func_801EDFBC_13FEDC(void);
extern s32 func_801EB728_13D648(void);
extern void func_801EB954_13D874(u8 opcode);
extern void func_801F45F4_146514(void);
extern void func_801EE7B8_1406D8(void);

/* ODD_C: this readiness predicate retains the ROM's xori result.
 * A direct != 1 test emits li v1,1 / beq v0,v1 instead. */
static inline s32 event_not_ready(void)
{
    return func_801EB728_13D648() ^ 1;
}

s32 func_801EF140_141060(s32 mode)
{
    u8 result = 4;
    if (mode) {
        D_801F51E8_147108->field990 = 6;
    }
    do {
        D_801F51E8_147108->field991 = 0;
        for (;;) {
            s32 event = (s16)func_801EE9A8_1408C8();
            if (event != 0) {
                D_801F51E8_147108->operands108.words[0] = event;
                D_801F51E8_147108->operands108.words[1] = 0;
                D_801F51E8_147108->field120 = 0;
                D_801F5CCC_147BEC = D_801F51E8_147108->field9F5;
                D_801F5CCD_147BED = 0x4C;
            } else {
                D_801F5CCD_147BED = func_801E53A4_1372C4(D_801F51EC_14710C->cursor++);
            }
            switch (D_801F5CCD_147BED & 0xF0) {
            case 0:
                result = func_801EBCA8_13DBC8();
                break;
            case 0x10:
                result = func_801ED008_13EF28();
                break;
            case 0x20:
                func_801ED720_13F640();
                break;
            case 0x30:
            case 0x40:
                result = func_801EDFBC_13FEDC();
                break;
            case 0x50:
                if (event_not_ready()) {
                    func_801EB954_13D874(D_801F5CCD_147BED);
                }
                break;
            case 0x60:
            case 0x70:
                func_801F45F4_146514();
                break;
            default:
                func_801EE7B8_1406D8();
                continue;
            }
            break;
        }
    } while (result == 4);
    return result;
}
