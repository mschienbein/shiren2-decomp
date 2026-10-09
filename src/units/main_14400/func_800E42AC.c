#include "common.h"

typedef struct Actor Actor;
typedef struct { unsigned char fields_00[0x80]; short field_80; void (*field_84)(void *, s32); short field_88; s32 (*field_8C)(void *, Actor *); } VTable;
struct Actor { unsigned char fields_00[0x1E]; unsigned char field_1E; unsigned char field_1F; u32 field_20; VTable *field_24; };
typedef struct { Actor *field_00; unsigned char fields_04[0xC]; unsigned char field_10; } Context;
extern Actor *D_801476B8;
extern unsigned short D_80156918;
extern unsigned short func_800E08B0(Actor *);
typedef struct { u32 field_00; } Flags;
static inline Flags *copy_flags(Flags *out, Actor *actor) { out->field_00 = actor->field_20; return out; }
static inline s32 evaluate(Actor *actor, Actor *target) { VTable *table = actor->field_24; return table->field_8C((unsigned char *)actor + table->field_88, target); }
static inline void apply(Actor *actor, s32 amount) { VTable *table = actor->field_24; table->field_84((unsigned char *)actor + table->field_80, amount); }
void func_800E42AC(Actor *actor, Context *context) {
    Actor *target = context->field_00;
    if (target && target != actor) {
        Flags flags;
        s32 special = (copy_flags(&flags, D_801476B8)->field_00 >> 23) & 1;
        if (special && ((target->field_1E >> 1) & 1)) target = D_801476B8;
        {
        s32 valid = 0;
        if (target->field_1E & 0x7C) valid = func_800E08B0(target) != 0;
        if (valid) {
            s32 amount = evaluate(actor, target);
            if (special && amount > 0) { amount = (amount * D_80156918) / 100; if (!amount) amount = 1; }
            apply(target, (amount * context->field_10) / 10);
        }
        }
    }
}
