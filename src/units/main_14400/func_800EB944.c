#include "common.h"
typedef unsigned char u8;
/* Embedded collection occupies 0xCC..0xE3; the iterator keeps its address at +4. */
typedef struct { void *pool; void *methods; u8 *entries; u8 state_C[4]; void *owner; unsigned short text_id; u8 pad_16[2]; } Collection;
typedef struct { u8 pad0[0xCC]; Collection collection; } Actor;
typedef struct { s32 index; Collection *collection; s32 active; void *current; } Iterator;
typedef struct { u8 unk0, unk1, unk2; } Entry;
extern u32 D_8013960C;
extern Iterator *func_800CEB20(Iterator *, Collection *);
extern s32 func_800CEBA0(Iterator *);
extern Entry *func_800CEC68(Iterator *);
extern void func_800AE518(Entry *, Actor *, s32, s32);
static inline s32 eligible(Entry *entry) { return (entry->unk2 & 4) && entry->unk0 != 9; }
s32 func_800EB944(Actor *arg0) {
    Iterator iterator;
    s32 changed = 0;
    D_8013960C <<= 1;
    func_800CEB20(&iterator, &arg0->collection);
    for (;;) {
        s32 valid = func_800CEBA0(&iterator);
        Entry *entry;
        if (!valid) break;
        entry = func_800CEC68(&iterator);
        if (eligible(entry)) {
            changed = 1;
            func_800AE518(entry, arg0, 0, 0);
        }
    }
    D_8013960C >>= 1;
    return changed;
}
