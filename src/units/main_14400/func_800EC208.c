#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x60]; s16 offset_60; s16 pad62; s32 (*fn_64)(void *self, void *item, s32 verbose); } TargetVTable800EC208;
typedef struct { s32 field_0; TargetVTable800EC208 *vtable_4; } Target800EC208;
/* Filled by func_800D0190: owning collection and the item handed to its +0x64 slot. */
typedef struct { Target800EC208 *target; Item *item; } Result800EC208;
typedef struct { s32 field_0; void *vtable_4; s32 field_8; s32 field_C; } Helper800EC208;
typedef struct { u32 pad : 8; u32 enabled : 1; u32 rest : 23; } Flags800EC208;
typedef struct {
    u8 pad0[0x20];
    Flags800EC208 flags_20;
    u8 pad24[0xC0];
    u16 flags_E4;
} Obj800EC208;
extern s32 D_80148090;
extern u8 D_80154300[];
Item *func_800E215C(Obj800EC208 *obj);
Helper800EC208 *func_800CFB60(Helper800EC208 *helper, Obj800EC208 *obj);
void *func_800D0190(void *output, void *helper, void *item);
s32 func_800EC384(Obj800EC208 *obj, Item *item);
void func_800498E4(s32 message_id, ...);

static inline s32 isIdle800EC208(void) {
    s32 idle = 0;

    if (D_80148090 == 0 || (D_80148090 ^ 3) == 0) {
        idle = 1;
    }
    return idle;
}

static inline s32 isBusy800EC208(Item *item, u8 kind) {
    s32 busy = 0;

    if (~item->owner05 != 0 ||
        (kind == 0x10 && item->field08->test1C((u8 *)item + item->field08->delta18, 0x23))) {
        busy = 1;
    }
    return busy;
}

static inline s32 isEnabled800EC208(Flags800EC208 *flags) {
    return flags->enabled;
}

void func_800EC208(Obj800EC208 *obj) {
    u16 flags = obj->flags_E4;
    s32 pending;
    Item *item;
    char *name;

    obj->flags_E4 = flags & ~0x80;
    pending = (flags >> 7) & 1;
    item = func_800E215C(obj);
    if (item == 0 || item->field00 == 0xF) {
        return;
    }
    name = func_800AE674(item);
    if (!pending && isIdle800EC208()) {
        Helper800EC208 helper;
        Result800EC208 result;
        Flags800EC208 objFlags;
        u8 kind;

        func_800CFB60(&helper, obj);
        func_800D0190(&result, &helper, item);
        kind = item->field00;
        if (!isBusy800EC208(item, kind)) {
            s32 flag = 0;

            if (kind != 0x10 && kind != 0xA) {
                flag = 1;
            } else {
                objFlags = obj->flags_20;
                if (isEnabled800EC208(&objFlags)) {
                    flag = 1;
                }
            }
            if (result.target->vtable_4->fn_64((u8 *)result.target + result.target->vtable_4->offset_60,
                                               result.item, flag)) {
                func_800EC384(obj, item);
            }
        }
        helper.vtable_4 = D_80154300;
    } else {
        func_800498E4(0x7A, name);
    }
    D_80148090 = 0;
}
