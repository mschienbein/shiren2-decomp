#include "common.h"

typedef struct {
    unsigned char pad_00[0x38];
    signed short delta_38;
    unsigned short reserved_3A;
    void *(*at_3C)(void *, u32);
} ListVtable;
typedef struct { void *items_00; ListVtable *vtable_04; } List;

/* D_80154390 slot 7 targets 800CE7A0: pointer result and unsigned index. */
void *func_800CE538(List *list, u32 index)
{
    ListVtable *vtable = list->vtable_04;
    return vtable->at_3C((char *)list + vtable->delta_38, index);
}
