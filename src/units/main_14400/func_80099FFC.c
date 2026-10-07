#include "common.h"
typedef struct { char pad[0x2D0]; s32 count; signed char ids[0x15]; } Obj;
s32 func_80099FFC(Obj *o, s32 id){ s32 i; for (i = 0; i < o->count; i++) { if (o->ids[i] == id) return 1; } return 0; }
