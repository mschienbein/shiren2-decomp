#include "common.h"
#include "collection_iterator.h"
typedef unsigned char u8;
/* CEB20/CEBA0/CEC68 share a 16-byte, word-aligned iterator. */
typedef struct { u8 unk0, unk1, unk2; } Entry;
static inline s32 has_id(Entry *entry, s32 id) {
    return (entry->unk2 & 4) && entry->unk1 == id;
}
Entry *func_800CF0CC(void *arg0, u8 arg1) {
    CollectionIterator iterator;
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
