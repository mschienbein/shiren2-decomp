#include "common.h"
typedef struct { char pad[2]; unsigned char field_2; } Obj;
static inline s32 nonzero(s32 value) { return value != 0; }
s32 func_800AF690(Obj *p) { return nonzero(p->field_2 & 0x40); }
