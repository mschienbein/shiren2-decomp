#include "common.h"
typedef struct { s32 field_0; void *field_4; } Entry;
typedef struct { u32 capacity,count,index; Entry *entries; } Queue;
void *func_8008D5A8(Queue *p) {
    void *result;
    if (p->count == 0) return 0;
    result = p->entries[p->index].field_4;
    p->index = (p->index + 1) % p->capacity;
    p->count--;
    return result;
}
