#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Damage record filled by func_80136910. */
typedef struct {
    void *source;
    u32 field_4;
    u32 field_8;
    u16 field_C;
    u16 field_E;
    u8 field_10;
} Damage;

extern u8 D_80147620[]; /* random number generator state */

s32 func_800C5844(void *rng, u8 base, u8 top);
void func_80136910(Damage *obj, void *a, u32 c, u32 b, u32 e);
s32 func_80049CB4(s32 id, ...);
void func_800A7ADC(void *target, Damage *damage);

/* Deal `amount` scaled by a random 90..110 percent from `actor` to `target`.
 * `self` is the adjusted object receiver that the original virtual-method chain supplies
 * (func_80117BC0 and func_80117CAC forward their own receiver in a0); it is unused here. */
void func_80117B00(void *self, void *actor, void *target, s16 amount)
{
    Damage damage;
    Damage *p = &damage;
    s16 value = amount * (u8)func_800C5844(D_80147620, 90, 110) / 100;

    func_80136910(p, actor, value, 0x1A, 0x808);
    func_80049CB4(0x72, target);
    func_800A7ADC(target, p);
}
