#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { void *field_0; u32 field_4, field_8; u16 field_C, field_E; u8 field_10; } Obj80136910;
typedef Obj80136910 Damage;
typedef struct Entity Entity;
extern unsigned char D_80147620[];
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_80136910(Obj80136910 *obj, void *a, u32 c, u32 b, u32 e);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A7ADC(Entity *, Damage *);
/* `self` is the adjusted object receiver that the original virtual-method chain supplies
 * (func_80119730 and func_80119830 forward their own receiver in a0); it is unused here. */
void func_80119670(void *self, void *source, void *target, short amount) {
    Damage damage;
    s32 scaled = amount * (u8)func_800C5844(D_80147620, 90, 110);
    Damage *message = &damage;
    func_80136910(message, source, (short)(scaled / 100), 0x1B, 0x808);
    func_80049CB4(0x72, target);
    func_800A7ADC(target, message);
}
