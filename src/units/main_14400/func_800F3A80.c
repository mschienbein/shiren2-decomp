#include "common.h"
typedef struct { char pad[0x9A]; unsigned short field_9a; } Obj;
static inline s32 nonzero(s32 value) { return value != 0; }
s32 func_800F3A80(Obj *p) { return nonzero(p->field_9a & 0x40); }
