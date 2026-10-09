#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;
typedef struct { s32 x, y; } Pos;
typedef struct { s16 delta; s16 pad; void (*destroy)(void *self, s32 flags); } DestroyEntry;
typedef struct { u8 pad0[8]; DestroyEntry destroy_8; } ItemVtable;
typedef struct { u8 kind_0, kind_1; u8 pad2[3]; s8 slot_5; u8 pad6[2]; ItemVtable *vtbl_8; u32 amount_C; } Item;
typedef struct { u8 data[0x1C]; } Collection;
typedef struct { Pos pos_0; u8 pad8[0x7C]; u32 money_84; u8 pad88[0xC]; u8 flags_94; u8 pad95[0x37]; Collection collection_CC; } Object;
extern u8 D_8013960A;
extern char *func_800AE674(void *item);
extern s32 func_800CD4C4(void *container, void *item);
extern void func_800AD868(Pos *pos);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_800EC0F4(void *obj, void *item);
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80123A80(void *object);
extern s32 func_800AC670(void *obj);
extern s32 func_800AD714(void *item, Pos *pos);
extern s32 func_800AD8AC(void *item, Pos *pos);
extern s32 func_800CD5C0(void *container, void *item);
extern s32 func_800CD538(void *container, void *item);
extern s32 func_800EC9F8(void *obj, u8 index);
extern void *func_800CDA70(void *list, u8 kind);
extern char *func_80048480(u16 id);
extern void func_800EC9D0(void *obj, u8 bit);

/* The item's kind byte.
 * ODD_C: the kind snapshot below reads through this accessor; returning the
 * byte type also lets combine keep the snapshot a plain register copy of known
 * width (a direct field read or (u8) cast leaves the later u16 message id
 * masked, see results.json). */
static inline u8 item_kind(Item *item) {
    return item->kind_1;
}

s32 func_800EC384(Object *obj, Item *item) {
    char *name = func_800AE674(item);
    /* ODD_C: `accepted` doubles as the kind snapshot of a stored item (one
     * variable, two roles), as `type` is reloaded after the pickup calls. */
    s32 accepted = 0;
    Pos pos;
    Pos *where;
    s32 type;

    type = item->kind_1;
    if (type == 0xCC || func_800CD4C4(&obj->collection_CC, item)) {
        accepted = 1;
    }
    /* FAKEMATCH: no-op pointer indirection; it keeps &pos in a saved register
     * across the five pickup/drop calls (s1). Using &pos directly lets GCC
     * rematerialize the stack address per call, shrinks the frame and
     * reallocates every saved register (166 words). An inline helper taking
     * &pos over the whole accepted path misses 2 words: the kind snapshot
     * moves into func_800CD538's delay slot (0x800EC544/0x800EC54C).
     * Measured by scratch/omp/jfix/j2/probe/EC384_by_address/match/result.json. */
    where = &pos;
    if (accepted) {
        where->x = obj->pos_0.x;
        where->y = obj->pos_0.y;
        func_800AD868(where);
        func_80049CB4(0x26, obj);
        func_80049CB4(0x25, item);
        func_800498E4(0x75, name);
        func_800EC0F4(obj, item);
        type = item->kind_1;
        if (type == 0xCC) {
            /* Money pickup: carry at most 999999, drop the excess as a new item. */
            u32 amount = item->amount_C;
            u32 total = obj->money_84 + amount;
            if (total <= 999999) {
                obj->money_84 = total;
            } else {
                Item *overflow = func_80123A80(func_800AC5B4(0x10, 0));
                s32 dropped = 0;
                if ((func_800AC670(overflow) ^ 1) != 0) {
                    if (func_800AD714(overflow, where)) {
                        func_80049CB4(0x122, where);
                        func_800AD8AC(overflow, where);
                        dropped = 1;
                    } else if (overflow) {
                        overflow->vtbl_8->destroy_8.destroy((u8 *)overflow + overflow->vtbl_8->destroy_8.delta, 3);
                    }
                }
                if (dropped) {
                    obj->money_84 = total - 1000000;
                } else {
                    obj->money_84 = 999999;
                }
            }
            if (item) {
                item->vtbl_8->destroy_8.destroy((u8 *)item + item->vtbl_8->destroy_8.delta, 3);
            }
        } else if (~item->slot_5 != 0) {
            func_800CD5C0(&obj->collection_CC, item);
        } else {
            s32 notify = 0;
            accepted = item_kind(item);
            func_800CD538(&obj->collection_CC, item);
            if (((obj->flags_94 >> 1) & 1) || ((obj->flags_94 & 1) && !func_800EC9F8(obj, accepted))) {
                notify = 1;
            }
            if (notify && func_800CDA70(&obj->collection_CC, accepted)) {
                s32 message = accepted + 0x9F2;
                s32 enabled = D_8013960A != 0;
                char *text;
                D_8013960A = 0;
                text = func_80048480(message);
                if (text != func_80048480(0x9F2)) {
                    func_800498E4(message);
                }
                func_800EC9D0(obj, accepted);
                if (enabled) {
                    D_8013960A = 1;
                }
            }
        }
        return 1;
    }
    func_800498E4(0x79, name);
    return 0;
}
