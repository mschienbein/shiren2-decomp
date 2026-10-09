#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
/* Slot +0x58/+0x5C: the caller func_80110258's class table D_8015D518 binds
 * u16 func_8011146C(Part800E8D0C *) here. */
typedef struct { u8 pad00[0x58]; s16 delta58, index5A; u16 (*amount5C)(void *); } ItemVTable;
typedef struct { u8 pad00[8]; ItemVTable *vtable08; } Item;
/* Slot +0x68/+0x6C: actor tables bind u32 func_800E0E88 / u32 func_800E96D4. */
typedef struct { u8 pad00[0x68]; s16 delta68, index6A; u32 (*bonus6C)(void *); } ActorVTable;
typedef struct { u8 pad00[0x1E]; u8 flags1E; u8 pad1F[5]; ActorVTable *vtable24; } Object;
typedef struct { void *source00; u32 kind04, flags08; s16 amount0C; u16 field0E; u8 field10; } Damage;
typedef Damage Obj80136910;
typedef struct Entity Entity;
extern void func_80136910(Obj80136910 *obj, void *a, u32 c, u32 b, u32 e);
extern s32 func_800E8DC8(Object *, s16);
extern void func_800A7ADC(Entity *, Damage *);

void func_80111128(Item *self, void *source, Object *actor, Entity *target) {
    Damage damage;
    s32 amount = self->vtable08->amount5C((u8 *)self + self->vtable08->delta58);
    func_80136910(&damage, source, (s16)amount, 6, 0);
    if (actor->flags1E & 0xC) {
        damage.amount0C = func_800E8DC8(actor, damage.amount0C);
    } else if (actor->flags1E & 0x7C) {
        damage.amount0C += actor->vtable24->bonus6C((u8 *)actor + actor->vtable24->delta68);
    }
    func_800A7ADC(target, &damage);
}
