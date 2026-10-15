#include "common.h"
#include "item_records.h"

/* Record iterator over pool D_80143094: the pool, then a halfword cursor. */
typedef struct { ItemPool *unk0; short unk4; } S;
S *func_800B07F0(S *s) { s->unk0 = &D_80143094; s->unk4 = 0; return s; }
