#include "common.h"
typedef unsigned char u8;
/* CEB20/CEBA0/CEC68 share a 16-byte, word-aligned iterator. */
typedef struct { s32 index; void *collection; s32 reverse; void *current; } Iterator;
typedef struct { u8 unk0, unk1, unk2; } Entry;
extern void *func_800CEB20(Iterator *, void *);
extern s32 func_800CEBA0(Iterator *);
extern Entry *func_800CEC68(Iterator *);
static inline s32 has_id(Entry *entry, s32 id) {
    return (entry->unk2 & 4) && entry->unk1 == id;
}
Entry *func_800CF0CC(void *arg0, u8 arg1) {
    Iterator iterator;
    func_800CEB20(&iterator, arg0);
    for (;;) {
        s32 valid = func_800CEBA0(&iterator);
        if (!valid) return 0;
        {
            Entry *entry = func_800CEC68(&iterator);
            if (has_id(entry, arg1 & 0xFF)) return entry;
        }
    }
}
