#include "common.h"

typedef struct { char pad[0x54]; signed char unk54[12]; } S;
s32 func_8009F608(S *s, s32 i) { return s->unk54[i]; }
