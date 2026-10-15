#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;

typedef struct Item Item;
/* g++ vtable slot {delta, index, pfn}. */
typedef struct { s16 delta, index; s32 (*call)(void *self); } ItemSlot;
typedef struct { u8 pad_00[0x10]; ItemSlot test; } ItemVtbl;
struct Item {
    u8 kind, id;
    u8 pad_02[6];
    ItemVtbl *vtable;
    union {
        s32 amount;
        struct { u8 count; s8 bonus; u8 level; u8 used; } stats;
    } u;
    u8 color_10;
};

extern u8 D_80148450[];
extern void func_801115D4(Item *item, s32 delta);
extern u16 func_800AE710(Item *item);
extern s16 func_800AE754(void *object, s16 add);
extern s32 func_8010E05C(Item *item, s32 add);
extern s32 func_8010BA90(Item *item, s16 v);
extern s32 func_8010BEC4(Item *item, u8 c);
extern s32 func_8010BDC4(Item *item, u8 value);
extern s32 func_8010BD3C(Item *item, s32 code);
extern s32 func_8010BF0C(void *item);
extern s32 func_8010BB60(Item *item, u8 *dst);
extern s32 func_8010BD00(Item *item, u8 value);
extern s32 func_800A2E48(Item *item);
extern s32 func_8010BCB0(Item *item, s8 delta);
extern s32 func_800A2910(u8 kind, u8 id, u16 *out);
extern void func_800AE55C(Item *item, s32 arg);

s32 func_800A2A5C(Item *dst, Item *src) {
    s32 type = dst->kind;
    u8 id = src->id;
    u8 srcKind = src->kind;
    u8 dstId = dst->id;
    Item *item;
    /* Zero-extended item id shared by cases 7 and 14 (one global pseudo). In case 14 it is live
       together with the 0xCC constant that local-alloc puts in v0, so global-alloc gives it v1 in
       both cases (ROM andi v1 at 0x800A2ACC and 0x800A2B3C). */
    s32 n;
    u8 kind = type;

    if (kind == srcKind) {
        /* FAKEMATCH (scratch/omp/rulings/A2A5C-SCOPED-EXCEPTION.md): dead store, id was just
           loaded from src->id. reload_cse deletes it, so it emits no instruction. At combine it
           sets mem_last_set, which invalidates the MEM last value of dstId's load, so combine
           no longer knows dstId's upper bits are zero and keeps case 7's zero-extend (n) instead
           of folding it into the compare. The ROM has no non-stack store or call before case 7.
           Measured: compliant best 1 word (range test reads n), 0 words with the exception;
           opus-04 result.json followup_a2a5c and followup_a2a5c_ladder (beam 24, permuter 800). */
        src->id = id;
        switch (kind) {
        case 7:
            /* Single-use n under scratch/omp/rulings/A2A5C-SCOPED-EXCEPTION.md: only the id compare
               reads it, while the range test reads raw dstId (ROM addiu v0,a0,0x68 at 0x800A2AD8).
               Sharing n with case 14 puts this mask in v1 (global-alloc conflict with case 14's
               0xCC constant in v0; .greg `87 conflicts: ... 2`), which frees v0 for the
               `move v0,zero` delay slot. Measured: compliant best 1 word (range test reads n),
               0 words with the exception; opus-04 result.json followup_a2a5c and
               followup_a2a5c_ladder. */
            n = dstId;
            if (n != id) {
                return 0;
            }
            if ((u8)(dstId + 0x68) < 2 && dst->color_10 != src->color_10) {
                return 0;
            }
            func_801115D4(dst, src->u.stats.count);
            return 1;
        case 5:
        case 11:
        case 12:
        case 13:
            if (dstId != id) {
                return 0;
            }
            func_800AE754(dst, func_800AE710(src));
            return 1;
        case 14:
            n = dstId;
            if (n == 0xCC && id == n) {
                func_8010E05C(dst, src->u.amount);
            }
            return 1;
        case 3:
        case 4: {
            u8 *code;
            s32 count;

            switch (id) {
            case 0x44:
            case 0x4B:
            case 0x4C:
            case 0x4D:
            case 0x50:
            case 0x6D:
            case 0x71:
                return 0;
            }
            item = dst;
            func_8010BA90(item, src->u.stats.bonus);
            if ((u8)func_8010BEC4(src, 0x22)) {
                func_8010BDC4(item, 0xF7);
                func_8010BD3C(src, 0x22);
            }
            if (func_8010BF0C(item)) {
                func_8010BDC4(src, 0xF7);
            }
            code = D_80148450;
            count = (u8)func_8010BB60(src, code);
            while (count-- > 0) {
                func_8010BD00(item, *code++);
                if (func_800A2E48(item)) {
                    return 1;
                }
            }
            return 1;
        }
        }
        return 0;
    }

    if ((u32)(type - 3) < 2) {
        s32 flag;
        u16 info[2];

        item = dst;
        flag = src->vtable->test.call((u8 *)src + src->vtable->test.delta) || id == 9 || id == 10;
        if (srcKind == 8 && id != 0xA4) {
            id = 0xA1;
        } else if (id == 0x15) {
            id = 5;
        }
        if (id == 0x20) {
            return func_8010BCB0(item, 1);
        }
        if (item->vtable->test.call((u8 *)item + item->vtable->test.delta) && id == 0x22) {
            func_8010BDC4(item, 0xF7);
            return 1;
        }
        if (!func_800A2910(kind, id, info)) {
            return 0;
        }
        if ((s8)(item->u.stats.level - item->u.stats.used) <= 0) {
            return 0;
        }
        func_8010BD00(item, id);
        if (func_800A2E48(item)) {
            return 1;
        }
        if (flag) {
            func_800AE55C(item, 1);
        }
        return 1;
    }
    return 0;
}
