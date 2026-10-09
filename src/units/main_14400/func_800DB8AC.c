#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    u8 pad0[0x60];
    s16 delta_60;
    s16 index_62;
    s32 (*can_extract_64)(void *list, void *item, s32 mode);
} ListVTable;

typedef struct {
    s32 field_0;
    ListVTable *vtable_4;
} List;

typedef struct {
    List *list;
    void *item;
} Link;

typedef struct {
    u8 pad0[8];
    Link link_8;
} Action;

extern void *D_801476B8;
s32 func_800EC384(void *unit, void *item);
/* Original returns the removed item, or null when the list removal fails. */
void *func_800D02AC(Link *link);

s32 func_800DB8AC(Action *action)
{
    Link *link = &action->link_8;
    List *list = action->link_8.list;
    if (list->vtable_4->can_extract_64((u8 *)list + list->vtable_4->delta_60,
                                     link->item, 1)) {
        if (func_800EC384(D_801476B8, link->item)) {
            func_800D02AC(link);
        }
    }
    return 0;
}
