#include "func_801EEC9C_140BBC/types.h"
extern S10EventState D_801F5288_1471A8, *D_801F51E8_147108;
extern S10ScriptFrame D_801F5C8C_147BAC[8], *D_801F51EC_14710C;
extern s8 D_80140160[];
extern s8 D_801F5CCC_147BEC, D_801F5CCE_147BEE;
extern s32 D_801F5CD0_147BF0, D_801F5CD8_147BF8;
extern S10Actor *D_801F5D08_147C28;
extern u8 D_801F5D0C_147C2C;
extern void func_800265E0(void *address, s32 size);
extern void func_80048380(void);
extern void func_801E5244_137164(void);
extern s32 func_801E5470_137390(u16 id);
extern void func_801E60F8_138018(void);
extern void func_801E6940_138860(void);
extern void func_801E69E8_138908(u8 mode);
extern void func_801E6E4C_138D6C(void);
extern void func_801E8170_13A090(void);
extern void func_801EC54C_13E46C(void);
extern void func_801EEBE4_140B04(S10Actor *actor, s32 frames);
extern void func_801F3238_145158(void);
extern void func_801F3544_145464(void);

void func_801EEC9C_140BBC(u16 id, s32 mode)
{
    D_801F5CD8_147BF8 = 1;
    D_801F51E8_147108 = &D_801F5288_1471A8;
    func_800265E0(&D_801F5288_1471A8, sizeof(D_801F5288_1471A8));
    D_801F51E8_147108->field9FB = 0xFF;
    func_801E60F8_138018();
    func_801E5244_137164();
    func_80048380();
    func_801EC54C_13E46C();
    D_801F5CCC_147BEC = 0;
    D_801F51EC_14710C = D_801F5C8C_147BAC;
    func_800265E0(D_801F5C8C_147BAC, sizeof(D_801F5C8C_147BAC[0]));
    D_801F51EC_14710C->id = id;
    D_801F51EC_14710C->cursor = func_801E5470_137390((u16)id);
    if (mode == 0) {
        if (D_80140160[6] == 1) {
            mode = 1;
        }
        if (D_80140160[6] == 2) {
            mode = 1;
        }
    }
    D_801F51E8_147108->field996 = mode;
    D_801F51E8_147108->field997 = 1;
    func_801F3238_145158();
    func_801F3544_145464();
    func_801E6E4C_138D6C();
    func_801E6940_138860();
    D_801F5CCE_147BEE = 1;
    D_801F5CD0_147BF0 = 0;
    func_801E8170_13A090();
    if ((D_801F5D0C_147C2C >> 1) & 1) {
        D_801F51E8_147108->field993 = 1;
        func_801E69E8_138908(1);
        if (D_801F5D08_147C28 != 0) {
            func_801EEBE4_140B04(D_801F5D08_147C28, 4);
        }
    }
}
