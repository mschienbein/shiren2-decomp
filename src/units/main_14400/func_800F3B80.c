#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad[0x9A]; u16 field9A; } Object;
static inline s32 nonzero(s32 value) { return value != 0; }
s32 func_800F3B80(Object *obj) { return nonzero(obj->field9A & 2); }
