#include "func_801EF6C4_1415E4/types.h"
/* Opaque byte serialization views; ROM 0x1415F4..0x141638 clears these
 * separate objects with lengths 0x40, 0x20, 0x10, 4 and 4. */
extern u8 D_801F5248_147168[];
extern u8 D_801F5228_147148[32];
extern u8 D_801F5218_147138[];
extern u8 D_801F5214_147134[];
extern u8 D_801F5210_147130[];
extern S10EventState *D_801F51E8_147108;
extern s32 D_801F5D04_147C24;
extern void func_800265E0(void *address, s32 size);
extern void func_801F2254_144174(u16 id, s32 flag);

void func_801EF6C4_1415E4(void)
{
    func_800265E0(D_801F5248_147168, 0x40);
    func_800265E0(D_801F5228_147148, 0x20);
    func_800265E0(D_801F5218_147138, 0x10);
    func_800265E0(D_801F5214_147134, 4);
    func_800265E0(D_801F5210_147130, 4);
    func_801F2254_144174(0, 0);
    D_801F51E8_147108 = 0;
    D_801F5D04_147C24 = 1;
}
