#include "common.h"
#include "item_records.h"

/* The item pool whose storage holds `item`, or 0. */
ItemPool *func_800AFB80(Item *item) { s32 i=0; for (;;) { ItemPool *pool; if (i>=7) break; pool=D_80153AE4[i]; if ((unsigned char)func_800AFD08(pool,item)!=255) return pool; i++; } return 0; }
