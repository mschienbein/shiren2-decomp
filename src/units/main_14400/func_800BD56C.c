#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
/* 0x14-byte room record: bounds words plus four per-side bytes (unused here). */
typedef struct { s32 data[4]; u8 pad10[4]; } Entry;
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
u8 func_800C57A0(void *);
u8 func_800C57CC(void *, s32);
s32 func_800A3138(Entry *);
s32 func_800A315C(Entry *);
s32 func_800BB22C(Obj *, Entry *);
s32 func_800BAFE4(Obj *, Entry *);
void func_800BD394(Obj *, Entry *);
void func_800BD428(Obj *, Entry *);
s32 func_800BD56C(Obj *obj) {
    Entry *entry;
    s32 tries;
    u8 idx;
    s32 checked;
    s32 hasA;
    s32 hasB;
    s32 failed;
    if (!(obj->flags & 0x10)) {
        return 0;
    }
    entry = 0;
    checked = func_800C57A0(D_80147620) & 1;
    tries = 100;
    while (1) {
        if (--tries == -1) {
            break;
        }
        idx = func_800C57CC(D_80147620, (u8)(obj->count - 1));
        if (obj->used[idx] == 0) {
            continue;
        }
        entry = &D_801431F0[idx];
        if (checked) {
            hasA = func_800A3138(entry);
            hasB = func_800A315C(entry);
            if (!(hasA & 1)) {
                failed = func_800BB22C(obj, entry) ^ 1;
                if (failed) {
                    continue;
                }
            }
            if (!(hasB & 1)) {
                failed = func_800BAFE4(obj, entry) ^ 1;
                if (failed) {
                    continue;
                }
            }
        }
        obj->used[idx] = 0;
        break;
    }
    if (tries < 0) {
        return 0;
    }
    if (checked) {
        func_800BD394(obj, entry);
    } else {
        func_800BD428(obj, entry);
    }
    return 1;
}
