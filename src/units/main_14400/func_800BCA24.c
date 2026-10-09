#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
/* 0x14-byte room record: bounds words plus four per-side bytes (unused here). */
typedef struct { s32 data[4]; u8 pad10[4]; } Entry;
typedef struct {
    char pad0[0x3DC];
    s32 count;
    char pad3E0[0x20];
    s8 mode;
    char pad401;
    u16 unk402;
    char pad404[0x958 - 0x404];
    u16 flags;
    char pad95A[2];
    s32 used[16]; /* func_800BA6A0 initializes sixteen room flags at 0x95C. */
} Obj;
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern Entry D_801431F0[];
extern s32 D_80147620[];
extern RoomState D_80143434;
u8 func_800C57CC(void *, s32);
void func_800D459C(void *, Entry *, s32, s32);
s32 func_800BCA24(Obj *obj) {
    Entry *entry = 0;
    s32 tries;
    u8 idx;
    if (!(obj->flags & 4)) {
        return 0;
    }
    tries = 100;
    while (1) {
        if (--tries == -1) {
            break;
        }
        idx = func_800C57CC(D_80147620, (u8)(obj->count - 1));
        if (obj->used[idx] != 0) {
            entry = &D_801431F0[idx];
            obj->used[idx] = 0;
            break;
        }
    }
    if (tries < 0) {
        return 0;
    }
    func_800D459C(&D_80143434, entry, obj->unk402 == 0x2000 && obj->mode != 2, obj->mode != 3);
    return 1;
}
