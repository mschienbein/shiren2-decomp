#include "common.h"

/*
 * g++ 2.8.1 constructor of a derived floor generator (C++ TU). Evidence: base
 * constructor func_800BA558 runs first, then the class vtable D_80153D58
 * (g++ {s16 delta, s16 index, pfn} entries) is stored at +8, then the 17-element
 * member array of an empty-constructor class is built (g++'s count-down array
 * construction loop), then the body runs and `this` is returned. C cannot
 * express the array-construction loop, so it is written with placement array new.
 */

typedef signed char s8;
typedef unsigned char u8;

inline void *operator new[](unsigned int, void *place) { return place; }

struct Pair {
    s32 x;
    s32 y;
};

struct Rectangle {
    Pair first;
    Pair second;
};

/* 0x14-byte room entry with an empty constructor; func_800C07FC reads the four tag bytes. */
struct Entry {
    Rectangle rectangle;
    u8 tag[4];
    Entry() {}
};

/* Partial view of the generator object (0x9CC bytes are touched here). */
struct Object {
    char fields_0[8];
    const void *vtable;
    char fields_C[0x3F4];
    s8 field_400;
    u8 field_401;
    short field_402;
    Entry entries_404[17];
    s32 links_558[16][16];
    short field_958;
    short field_95A;
    s32 enabled_95C[16];
    Rectangle bounds_99C;
    Rectangle bounds_9AC;
    Rectangle bounds_9BC;
};

extern "C" {
extern Rectangle D_801429B0;
extern u8 D_80153D58[];
Object *func_800BA558(Object *self, u8 kind, s8 variant, s32 flag, s32 value);
s32 func_800ABD24(void);
}

static inline void initialize_rectangle(Rectangle *destination)
{
    destination->first = D_801429B0.first;
    destination->second = D_801429B0.second;
}

static inline void enable_entries(Object *object, s32 enabled)
{
    s32 i;
    for (i = 15; i >= 0; i--) {
        object->enabled_95C[i] = enabled;
    }
}

static inline int entry_in_range(s32 index) { return index < 17; }

extern "C" Object *func_800BA6A0(Object *self, u8 kind, s8 index, s8 variant, s32 flag, s32 value)
{
    func_800BA558(self, kind, variant, flag, value);
    self->vtable = D_80153D58;
    new (self->entries_404) Entry[17];
    if (index >= 0) {
        self->field_400 = index;
    } else {
        self->field_400 = func_800ABD24();
    }
    self->field_402 = 0x2000;
    enable_entries(self, 1);
    self->field_958 = 0;
    initialize_rectangle(&self->bounds_99C);
    initialize_rectangle(&self->bounds_9AC);
    initialize_rectangle(&self->bounds_9BC);
    for (s32 entry = 0; entry_in_range(entry); entry++) {
        initialize_rectangle(&self->entries_404[entry].rectangle);
    }
    /* ODD_C: The 16x16 link words (lw/sw in func_800C0304/func_800C056C/func_800C07FC) are
     * cleared byte by byte here: ROM 0x8D86C is an sb loop over all 0x400 bytes. */
    u8 *data = (u8 *)self->links_558;
    s32 remaining = 1023;
    do {
        *data++ = 0;
    } while (--remaining != -1);
    return self;
}
