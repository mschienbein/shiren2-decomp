#include "common.h"
#include "item_records.h"

typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[0x28]; VEntry e; } VTable;
typedef struct { char pad[0x18]; VTable *vtbl; } B;
typedef void (*Fn3)(void *, s32, void *);
extern char D_80153B00[];
extern char D_80147620[];
extern const unsigned char D_8015488C[8];
void func_800CA4E8(B *, void *);
void func_800C56D4(void *);
void func_800C573C(void *);
void *func_800AC244(unsigned char id);
/* Load a pool saved by func_800B0054: each live record is rebuilt in place by the item
 * factory while D_80143104 publishes the record to reuse. */
void func_800B0164(ItemPool *pool, B *b) {
    s32 i;
    VEntry *e;
    func_800CA4E8(b, D_80153B00);
    e = &b->vtbl->e;
    ((Fn3)e->fn)((char *)b + e->delta, (pool->capacity + 7) / 8, pool->occupancy);
    i = 0;
    func_800C56D4(D_80147620);
    for (;; i++) {
        s32 n = pool->capacity;
        if (i >= n) break;
        if (pool->occupancy[i >> 3] & D_8015488C[i & 7]) {
            unsigned char id;
            Item *o;
            ((Fn3)b->vtbl->e.fn)((char *)b + b->vtbl->e.delta, 1, &id);
            /* local-arithmetic-qualification: pool->items + i emits the
             * reversed addu operands at 0x800B0234 (v0,s1 rather than s1,v0).
             * Only this address calculation uses an integer; the storage and
             * the published record override remain typed pointers. */
            D_80143104 = (Item *)(i * sizeof(Item) + (u32)pool->items);
            o = func_800AC244(id);
            o->field08->load2C((char *)o + o->field08->delta28, b);
        }
    }
    D_80143104 = 0;
    func_800C573C(D_80147620);
}
