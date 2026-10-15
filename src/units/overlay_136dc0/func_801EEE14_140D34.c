#include "func_801EEE14_140D34/types.h"
extern S10EventState *D_801F51E8_147108;
extern S10Actor *D_801F5D08_147C28;
extern S10Actor *D_801476B8;
extern S10SelectionSave D_80142F24;
extern s8 D_80140160[];
extern u8 D_801F5D0C_147C2C, D_801F5CCE_147BEE;
extern u16 D_801F5D0E_147C2E, D_801F5D10_147C30;
extern s32 D_801F5CD0_147BF0, D_801F5CD8_147BF8;
extern void func_801F2408_144328(void);
extern void func_801EEA38_140958(void);
extern void func_801E7850_139770(void);
extern void func_801E8634_13A554(s32 x, s32 y, s32 z);
extern void func_801E7414_139334(void);
extern void func_801E6968_138888(void);
extern u8 func_801E9F70_13BE90(s32 flag);
extern void func_801F356C_14548C(void);
extern void func_801E9F90_13BEB0(s32 flag, u8 value);
extern void func_801E665C_13857C(void);
extern void func_801E6484_1383A4(void);
extern void func_801F32A8_1451C8(void);
extern void func_801E60F8_138018(void);
extern void func_801E7AD8_1399F8(void);
extern void func_800838A8(u8 value);
extern void func_801E700C_138F2C(void);
extern void func_800485C0(void);
extern void func_801E8170_13A090(void);
extern void func_801E6688_1385A8(s32 mode);
extern void func_801E7E20_139D40(s32 value);
extern void func_801E7FB4_139ED4(void);
extern void func_801EC510_13E430(void);
extern void func_80048380(void);
extern void func_801F1FF4_143F14(void);
extern void func_801EF0F0_141010(void);
extern void func_800C9820(s32 index, s32 count);
extern void func_800C9800(s32 value);
extern void func_80094B3C(void *object, s32 mode);
extern void func_800C9870(void);
extern s32 func_801E7C14_139B34(void);
extern void func_80046A40(s32 mode);
extern void func_800AA944(void);
extern void func_800C92BC(void *actor);
extern void func_80045E50(void);

/* ODD_C: scalar accessors retain independent absolute field loads without
 * inventing an interior D_80142F25 object. The direct aggregate expression
 * coalesces the base address and moves the second load into the delay slot. */
static inline u8 selection_index(S10SelectionSave *selection)
{
    return selection->index;
}

static inline u8 selection_count(S10SelectionSave *selection)
{
    return selection->count;
}

void func_801EEE14_140D34(s32 reason)
{
    u16 id = D_801F5D0E_147C2E;
    u8 wait = D_801F51E8_147108->field9FB;
    if (D_801F51E8_147108->field993 == 1) {
        D_801F51E8_147108->field993 = 0;
    }
    if (wait == 0xFF) {
        wait = 1;
        if (((D_801F5D0C_147C2C >> 1) & 1) && D_801F5D08_147C28) {
            s32 amount = 0;
            if ((D_801F5D08_147C28->flags1E >> 6) & 1) {
                amount = ((S10ActorA *)D_801F5D08_147C28)->component78.field2C;
            }
            if ((D_801F5D08_147C28->flags1E >> 3) & 1) {
                amount = ((S10ActorB *)D_801F5D08_147C28)->component84.field2C;
            }
            if (amount) {
                wait = 0;
            }
        }
    }
    func_801F2408_144328();
    func_801EEA38_140958();
    func_801E7850_139770();
    func_801E8634_13A554(0, 0, 0);
    func_801E7414_139334();
    func_801E6968_138888();
    if (!func_801E9F70_13BE90(0x4C)) {
        func_801F356C_14548C();
    }
    func_801E9F90_13BEB0(0x4C, 0);
    func_801E665C_13857C();
    func_801E6484_1383A4();
    func_801F32A8_1451C8();
    func_801E60F8_138018();
    func_801E7AD8_1399F8();
    func_800838A8(1);
    func_801E700C_138F2C();
    func_800485C0();
    D_801F5CCE_147BEE = 0;
    func_801E8170_13A090();
    func_801E6688_1385A8(1);
    func_801E7E20_139D40(1);
    func_801E7FB4_139ED4();
    func_801EC510_13E430();
    func_80048380();
    if (D_801F5CD0_147BF0) {
        D_801F5CD0_147BF0 = 0;
        func_801F1FF4_143F14();
    }
    /* ODD_C: these five reasons suppress the default cleanup, rather than
     * being filler cases. Both ROM dispatches test 8 then the range 10..13.
     * The equivalent inequality candidate emitted 704 bytes; the switch
     * expresses this discontiguous suppression set without range tricks. */
    switch (reason) {
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    default:
        if (D_801F51E8_147108->field9F0) {
            if (wait && id != 0x2C7) {
                func_801EF0F0_141010();
            }
            func_800C9820(selection_index(&D_80142F24), selection_count(&D_80142F24));
            func_800C9800(1);
            func_80094B3C(D_80140160, 2);
            func_800C9870();
        }
        if (!D_801F5D10_147C30 && (u8)func_801E7C14_139B34()) {
            func_80046A40(0x10);
        }
        break;
    }
    D_801F51E8_147108 = 0;
    func_800AA944();
    func_800C92BC(D_801476B8);
    switch (reason) {
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    default:
        if (wait && id != 0x2C7) {
            func_801EF0F0_141010();
        }
        break;
    }
    func_80045E50();
    D_801F5CD8_147BF8 = 0;
}
