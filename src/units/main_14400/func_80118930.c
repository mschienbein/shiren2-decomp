#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { unsigned int prefix:23; unsigned int blocked:1; unsigned int rest:8; } Flags;
typedef struct { short adjustment, unused; s32 (*call)(void *, s32, s32, u8, s32); } Slot;
typedef struct { u8 pad_00[0x90]; Slot effect_90; } Table;
typedef struct { u8 pad_00[0x20]; Flags flags_20; Table *vtable_24; } Actor;
typedef struct { void *source; s32 type, auxiliary; short amount; u16 options; u8 scale; } Damage;
extern short D_801569EE;
extern u16 D_801569EC;
extern s32 func_800E1CC4(void *, s32);
extern s32 func_800E0AB4(void *, s32);
extern s32 func_80049CB4(s32, ...);
extern void func_80136910(Damage *, void *, u32, u32, u32);
extern void func_800A7ADC(void *, Damage *);
/* ODD_C: inline value helpers retain the original flag and damage temporaries; the
   effect factor stays a u8 (1 or 2) through every helper, as in the original. */
static inline Flags *read_flags(Flags *out, Actor *actor) { *out = actor->flags_20; return out; }
static inline s32 allowed(Flags *flags) { return flags->blocked != 1; }
static inline u8 effect_factor(s32 active) {
    return active ? 2 : 1;
}
static inline Damage *create_damage(Damage *damage, u8 factor) {
    func_80136910(damage, 0, (short)(D_801569EC * factor), 0x18, 8);
    return damage;
}
static inline void change_effect(Actor *actor, u8 factor) {
    Slot *slot = &actor->vtable_24->effect_90;
    void *adjusted = (u8 *)actor + slot->adjustment;
    s32 amount = -factor;
    s32 kind = amount > 0 ? 0x12 : 0x11;
    slot->call(adjusted, 0, kind, 0xFE, amount);
}
void func_80118930(void *self /* unused callback receiver */, Actor *actor) {
    Damage damage;
    Flags flags;
    u8 factor = effect_factor(func_800E1CC4(actor, 3));
    if (allowed(read_flags(&flags, actor))) {
        s32 adjustment = factor * D_801569EE;
        func_800E0AB4(actor, adjustment);
        func_80049CB4(0x128, 0x78);
    }
    change_effect(actor, factor);
    func_800A7ADC(actor, create_damage(&damage, factor));
}
