#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xC];
    u32 count;
} Item800F1568;

typedef struct {
    u8 pad0[0x72];
    u8 flags;
    u8 pad73[0x81 - 0x73];
    u8 key;
    u16 itemId;
} Obj800F1568;

extern u8 D_80147620[];
extern u16 D_8015690E;
s32 func_800C587C(void *table, u8 key);
Item800F1568 *func_800AACE0(s32 arg);
Item800F1568 *func_801239D4(void);
s32 func_800AC670(Item800F1568 *item);
u16 func_800AB044(void);
Item800F1568 *func_800AADB4(u8 id, s32 arg);
Item800F1568 *func_800AADF8(s8 id, u8 arg);
Item800F1568 *func_800AAEB4(s8 id);

Item800F1568 *func_800F1568(Obj800F1568 *obj, s32 force) {
    Item800F1568 *item = 0;
    s32 allowed = 0;
    u16 id;
    s32 ok;
    s32 kind;

    if ((obj->flags & 8) && (force || func_800C587C(D_80147620, obj->key))) {
        allowed = 1;
    }
    if (allowed) {
        kind = obj->itemId;
        id = kind;
        if (id == 0x100) {
            item = func_800AACE0(0);
        } else if (id == 0x101) {
            item = func_801239D4();
            ok = func_800AC670(item) != 1;
            if (ok) {
                u32 count = func_800AB044() * 3;

                if (count > D_8015690E) {
                    count = D_8015690E;
                }
                item->count = count;
            } else {
                item = 0;
            }
        } else if ((u32)(kind - 1) < 0xF7) {
            item = func_800AADB4(id, 1);
        } else if ((u32)(kind - 0x102) < 10) {
            item = func_800AADF8(id - 2, 0);
        } else if ((u32)(kind - 0x10C) < 4) {
            item = func_800AAEB4(id - 0xC);
        }
    }
    return item;
}
