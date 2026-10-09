#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x38]; short adjust_38; short pad_3A; void *(*get_3C)(void *, u32); } ListVTable;
typedef struct { void *pool_00; ListVTable *vtable_04; } List;
typedef struct { u8 pad_00[0x98]; short adjust_98; short pad_9A; List *(*list_9C)(void *); } ActorVTable;
typedef struct { u8 pad_00[0x24]; ActorVTable *vtable_24; } Actor;
/* func_8009A9D0 populates at most two indices, followed by count at +0x58. */
typedef struct { u8 pad_00[0x50]; u32 indices_50[2]; s32 count_58; } Object;
extern Actor *D_801476B8;
/* Targets: D_80159008+0x9C=func_800EE07C; D_80154438+0x3C=func_800CE7A0. */
void *func_8009AACC(Object *self, s32 slot) {
    Actor *actor = D_801476B8;
    List *list = actor->vtable_24->list_9C((u8 *)actor + actor->vtable_24->adjust_98);
    return list->vtable_04->get_3C((u8 *)list + list->vtable_04->adjust_38, self->indices_50[slot]);
}
