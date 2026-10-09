#include "common.h"

typedef unsigned char u8;

typedef struct Stream Stream;

/* Resource callbacks registered by func_8008D3A0 (types as in func_800905DC). */
typedef void (*ResInitFn)(void);
typedef s32 (*ResLoadFn)(void *res, void *owner, void *ctx);
typedef void (*ResFreeFn)(void *res);

typedef struct { s32 x0; s32 x4; } Entry;

/* 0x20-byte 'ANMF' resource: registration header, then the entry table read by
 * func_80090A04. */
typedef struct {
    s32 tag;
    s32 id;
    ResInitFn init;
    ResLoadFn load;
    ResFreeFn release;
    s32 field_14;
    u32 count;
    Entry *entries;
} Anim;

/* Owner collection: count at +2, record pointer array at +8. */
typedef struct {
    u8 pad0[2];
    u8 count;
    u8 pad3[5];
    Anim **records;
} Collection;

#define TAG_ANMF 0x414E4D46

void *func_80091450(u32 size);
u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_8008D3A0(Anim *rec, s32 tag, ResInitFn init, ResLoadFn load, ResFreeFn release);
s32 func_8008DF04(Stream *stream);
s32 func_80090A04(Stream *stream, Anim *anim);
void func_80091544(void *item);
void func_80090940(void);
s32 func_80090948(void *res, void *owner, void *ctx);
void func_800909E8(void *res);

/* Reads one animation resource whose encoded length must be `expected` bytes and appends
 * it to the collection; returns 0, or -1 (freeing the record) on failure. */
s32 func_80090AB4(Stream *stream, s32 expected, Collection *collection)
{
    s32 status = 0;
    s32 consumed;
    Anim *anim;

    /* ODD_C: early-exit group; each failure breaks to the shared free of the record. Also shapes
     * scheduling: the if/else-if chain and goto-done forms each differ in 3 words. */
    do {
        anim = func_80091450(sizeof(Anim));
        if (anim == 0) {
            status = -1;
            break;
        }
        func_8006A810((u8 *)anim, 0, sizeof(Anim));
        func_8008D3A0(anim, TAG_ANMF, func_80090940, func_80090948, func_800909E8);
        anim->field_14 = func_8008DF04(stream);
        anim->count = func_8008DF04(stream);
        consumed = func_80090A04(stream, anim);
        if (consumed < 0) {
            status = -1;
            break;
        }
        if (expected != consumed + 8) {
            status = -1;
            break;
        }
        collection->records[collection->count++] = anim;
    } while (0);
    if (status && anim) func_80091544(anim);
    return status;
}
