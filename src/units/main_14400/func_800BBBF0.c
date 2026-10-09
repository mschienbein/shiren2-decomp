#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* 0x14-byte room record: bounds rectangle plus four per-side bytes (unused here). */
typedef struct {
    s32 data[4];
    u8 pad10[4];
} Entry;

/* Partial view of the floor generator object. */
typedef struct {
    char pad0[0x3DC];
    s32 count;
    char pad3E0[0x958 - 0x3E0];
    u16 flags;
    char pad95A[2];
    s32 used[16];
} Obj;

extern Entry D_801431F0[];
extern s32 D_80147620[];
u8 func_800C57CC(void *rng, s32 limit);
s32 func_800B68B0(Entry *entry);
s32 func_800BB6C0(void *ctx, Entry *room);

void func_800BBBF0(Obj *obj)
{
    u8 wanted;
    u8 placed;
    s32 tries;

    if (!(obj->flags & 0x1000)) {
        return;
    }
    wanted = func_800C57CC(D_80147620, 2);
    if (wanted == 0) {
        return;
    }
    placed = 0;
    tries = 100;
    while (1) {
        u8 idx;
        Entry *entry;

        if (--tries == -1) {
            break;
        }
        idx = func_800C57CC(D_80147620, (u8)(obj->count - 1));
        if (obj->used[idx] == 0) {
            continue;
        }
        entry = &D_801431F0[idx];
        if (!func_800B68B0(entry)) {
            continue;
        }
        if (!func_800BB6C0(obj, entry)) {
            continue;
        }
        obj->used[idx] = 0;
        if (++placed == wanted) {
            break;
        }
    }
}
