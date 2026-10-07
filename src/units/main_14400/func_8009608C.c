#include "common.h"

typedef struct { s32 x; s32 y; } Pair;
typedef struct { char pad[0x3C]; Pair value; } Object;
void func_8009608C(Object *obj, Pair *value) { obj->value=*value; }
