#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Entity vtable (vptr at +0x24, e.g. D_8015CB48): {delta, pad, pfn} entries. */
typedef struct { s16 delta; s16 pad; s32 (*fn)(void *self); } Query;
typedef struct { s16 delta; s16 pad; void (*fn)(void *self); } Action;
typedef struct { s16 delta; s16 pad; s32 (*fn)(void *self, void *target); } Query1;
typedef struct { s16 delta; s16 pad; s32 (*fn)(void *self, void *item, void *source); } Query2;
typedef struct {
    u8 pad00[0x10];
    Query isActive_10;
    u8 pad18[0xA0];
    Action reset_B8;
    u8 padC0[0x8];
    Query check_C8;
    Query1 handle_D0;
    u8 padD8[0x8];
    Query2 handleItem_E0;
    Query2 handleHit_E8;
} EntityVtable;

typedef struct Object Object;
struct Object {
    u8 pad00[0xA];
    u8 id_0A;
    u8 pad0B[0x13];
    u8 flags_1E;
    u8 pad1F[5];
    EntityVtable *vtbl_24;
    u8 pad28[0x30];
    Object *target_58;
    u8 pad5C[0x5E];
    u8 field_BA, field_BB, field_BC;
};
typedef struct { Object *source; u32 kind; } Damage;
typedef struct { u32 kind; void *value; void *item; s32 pad0C; Damage *damage; } Event;

extern const u8 D_801531DC[];
extern s32 func_800EEF08(void *actor);
extern s32 func_800E0F40(void *obj);
extern s32 func_800E4454(void *unit);
extern s32 func_800A533C(void *obj);
extern u16 func_800E08B0(void *obj);
extern void func_800E4470(void *obj);
extern void func_800498E4(s32 id, ...);
extern void func_800EEE28(void *self, void *arg);
extern char *func_800A3B20(void *obj);
extern s32 func_800EA67C(void *self, void *event);
extern s32 func_800EF318(void *self, void *value);

s32 func_800EEBC4(Object *self, Event *event) {
    switch (event->kind) {
    case 4:
        self->vtbl_24->reset_B8.fn((u8 *)self + self->vtbl_24->reset_B8.delta);
        return 1;
    case 0:
        return func_800EEF08(self);
    case 1: {
        Object *target = self->target_58;
        s32 result = self->vtbl_24->check_C8.fn((u8 *)self + self->vtbl_24->check_C8.delta);
        if (result) {
            s32 special = 0;
            if (target) {
                if (target->vtbl_24->isActive_10.fn((u8 *)target + target->vtbl_24->isActive_10.delta)) {
                    special = (target->flags_1E >> 4) & 1;
                }
            }
            if (special) {
                self->field_BB = target->id_0A;
                self->field_BC = func_800E0F40(target);
                self->field_BA = 0;
            }
        }
        return result;
    }
    case 25:
        return self->vtbl_24->handle_D0.fn((u8 *)self + self->vtbl_24->handle_D0.delta, event->value);
    case 9:
        func_800EA67C(self, event);
        if (func_800E08B0(self)) {
            Damage *damage = event->damage;
            if (damage->source) {
                s32 special = 0;
                if ((damage->source->flags_1E >> 2) & 1) {
                    special = func_800E4454(self) != 0;
                }
                if (special) {
                    func_800E4470(self);
                    func_800498E4(0x226, func_800A3B20(self));
                    func_800A533C(self);
                } else if ((damage->source->flags_1E >> 1) & 1) {
                    self->target_58 = damage->source;
                }
            }
            {
                u32 index = event->damage->kind - 7;
                if (index < 13) {
                    self->field_BC = 0;
                    self->field_BA = 0;
                    self->field_BB = D_801531DC[index];
                }
            }
        }
        return 1;
    case 13:
        return func_800EF318(self, event->value);
    case 15:
        return self->vtbl_24->handleItem_E0.fn((u8 *)self + self->vtbl_24->handleItem_E0.delta, event->value, event->item);
    case 16:
        return self->vtbl_24->handleHit_E8.fn((u8 *)self + self->vtbl_24->handleHit_E8.delta, event->value, event->damage);
    case 10:
        func_800EEE28(self, event);
        return 1;
    default:
        return func_800EA67C(self, event);
    }
}
