#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct VtEntry {
    s16 delta;
    s16 index;
    void *pfn;
} VtEntry;

/* Collection/item link (func_800D01B8 shape); func_800DC1CC fills the +0x10 link of this
 * class (D_80158598) through func_800DAA58. */
typedef struct Entry800D01B8 {
    void *collection;
    void *item;
} Entry800D01B8;

typedef struct Obj800DBFF0 {
    u8 pad_00[0x4];
    const VtEntry *vtable_04;
    u8 pad_08[0x8];
    Entry800D01B8 pos_10;
} Obj800DBFF0;

extern const VtEntry D_80158598[];
extern void *func_800DA8A0(Obj800DBFF0 *obj, s32 kind, Entry800D01B8 *source);
Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);

/* Constructor: base kind 0x16 copying the source link, vtable D_80158598, then construct and
 * assign the second link member. */
Obj800DBFF0 *func_800DBFF0(Obj800DBFF0 *obj, Entry800D01B8 *source, Entry800D01B8 *pos) {
    func_800DA8A0(obj, 0x16, source);
    obj->vtable_04 = D_80158598;
    func_800D0180(&obj->pos_10);
    obj->pos_10 = *pos;
    return obj;
}
