#include "common.h"
typedef struct { s32 key; void *val; } Entry;
typedef struct { u32 cap; u32 count; u32 head; Entry *entries; } Queue;
s32 func_8008D4B8(Queue *q, void *val, s32 key) {
    s32 prev, slot, next;
    u32 last;
    u32 i;
    if (q->count >= q->cap) return -1;
    last = q->head - 1;
    prev = (last + q->count) % q->cap;
    slot = (q->head + q->count) % q->cap;
    for (i = 0; i < q->count; i++) {
        if (!(q->entries[prev].key < key)) break;
        q->entries[slot].key = q->entries[prev].key;
        q->entries[slot].val = q->entries[prev].val;
        slot = prev;
        if (slot > 0) next = slot - 1;
        else next = q->cap - 1;
        prev = next;
    }
    q->entries[slot].key = key;
    q->entries[slot].val = val;
    q->count++;
    return 0;
}
