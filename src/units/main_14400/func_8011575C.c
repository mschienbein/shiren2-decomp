#include "common.h"

/* Flag word at +0x20; only bit 23 (ninth from the top) is read. */
typedef struct {
    unsigned high : 8;
    unsigned flag : 1;
    unsigned low : 23;
} Flags;

typedef struct {
    char pad0[0x20];
    Flags flags;
} Obj;

/* One complete four-byte flags snapshot; no unused frame-shaping storage. */
typedef struct {
    Flags flags;
} Snapshot;

extern Obj *D_801476B8;

static __inline__ Flags read_flags(Obj *obj)
{
    return obj->flags;
}

u32 func_8011575C(void *self)
{
    Snapshot snapshot;
    u32 result;

    snapshot.flags = read_flags(D_801476B8);
    result = snapshot.flags.flag;
    return result;
}
