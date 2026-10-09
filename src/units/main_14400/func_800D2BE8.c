#include "common.h"
typedef struct { char pad0[0xC]; s32 fieldC; } Entry800D3650;
typedef struct Ent Ent;
s32 func_800D2BDC(signed char *arg);
s32 func_800AE9AC(Ent *, s32, s32);
void func_800D2BE8(Entry800D3650 *entry, void *arg) { s32 amount = func_800D2BDC((signed char *)entry); entry->fieldC += func_800AE9AC(arg, amount, 0); }
