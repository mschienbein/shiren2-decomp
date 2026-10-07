#include "common.h"

typedef struct { char pad[0x9A]; unsigned short flags; } Object;
s32 func_800F3B30(Object *obj) { if (obj->flags & 8) return 1; return 0; }
