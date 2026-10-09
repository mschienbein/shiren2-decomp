#include "common.h"
typedef struct { void *container; void *item; } Entry;
extern Entry func_80098018(void *object, s32 index);
void *func_800980F0(void *object, s32 index) {
    Entry entry;
    entry = func_80098018(object, index);
    return entry.item;
}
