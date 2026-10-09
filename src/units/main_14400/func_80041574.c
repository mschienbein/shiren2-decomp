#include "common.h"
typedef struct { s32 field_00; s32 field_04; } Pair;
extern s32 func_800B200C(Pair *pair);
s32 func_80041574(s32 a, s32 b) { Pair pair; pair.field_04 = a; pair.field_00 = b; return func_800B200C(&pair); }
