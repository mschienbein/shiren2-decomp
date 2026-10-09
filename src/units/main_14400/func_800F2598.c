#include "common.h"

typedef unsigned char u8;
typedef struct { short delta, index; void (*call)(void *, short); } ShortEntry;
typedef struct { short delta, index; void (*call)(void *, s32); } IntEntry;
typedef struct { char pad0[0x78]; ShortEntry adjust_78; IntEntry forward_80; } Vtable;
typedef struct Object { char pad0[0x24]; Vtable *vtbl_24; char pad28[0x44]; s32 field_6C; char pad70[4]; u8 field_74; char pad75[0x8F]; struct Object *field_104; } Object;
extern Object *D_801476B8;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern unsigned short func_800E08B0(void *obj);

static inline s32 modeIs4F(void) { return D_80142F18.mode == 0x4F; }

void func_800F2598(Object *obj, s32 amount) {
    s32 forwarded;
    s32 suppressed = 0;
    if (func_800E08B0(obj) == 0 || modeIs4F()) {
        suppressed = 1;
    }
    if (!suppressed) {
        Object *actor = D_801476B8;
        forwarded = actor->field_104 != 0 && obj == actor->field_104;
        if (forwarded) {
            IntEntry *entry = &actor->vtbl_24->forward_80;
            entry->call((char *)actor + entry->delta, amount);
        } else if (obj->field_74) {
            if (amount > 0) {
                amount = 1;
            } else if (amount < 0) {
                amount = -1;
            }
            obj->field_6C += amount;
        } else {
            ShortEntry *entry = &obj->vtbl_24->adjust_78;
            entry->call((char *)obj + entry->delta, (short)amount);
        }
    }
}
