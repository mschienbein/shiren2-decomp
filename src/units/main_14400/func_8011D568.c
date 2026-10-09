#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef struct { u8 pad00[0x1E]; u8 flags1E; } Unit;
/* Complete 0x14-byte damage record written by func_801124F8. */
typedef struct { void *actor; s32 value04, value08; s16 amount; u16 flags; u8 kind; } Damage;
extern u8 D_80147620[];
/* Adjacent halfword tuning values; 0x801569B7 is the low byte of chance. */
typedef struct { u16 multiplier; u16 chance; } DamageTuning;
extern const DamageTuning D_801569B4;
/* a1 is the attacker's pointer, copied into the damage record's first word. */
extern void func_801124F8(void *, Unit *, Unit *, Damage *);
extern s32 func_800C587C(void *, u8);
extern void func_800498E4(s32, ...);
extern void func_800A7ADC(Unit *, Damage *);
void func_8011D568(void *source, Unit *attacker, Unit *target, Unit *unit)
{
    Damage damage;
    func_801124F8(source, attacker, unit, &damage);
    if (func_800C587C(D_80147620, (u8)D_801569B4.chance) != 0) {
        s32 message = 0x125;
        if (attacker != 0 && (attacker->flags1E & 0xC)) {
            message = 0x3A;
        }
        func_800498E4(message);
        damage.amount = damage.amount * D_801569B4.multiplier / 10;
        damage.flags |= 0x80;
    }
    func_800A7ADC(target, &damage);
}
