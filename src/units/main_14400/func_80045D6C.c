#include "common.h"

typedef struct { s32 unk0; void *obj; } Handle;
void *func_800CB330(Handle *); s32 func_800CB33C(Handle *); void func_800CB408(Handle *); void func_800CB4D4(Handle *);
void func_80045D6C(s32 a) { Handle h; func_800CB330(&h); func_800CB33C(&h); if (a == 1) h.unk0 = a; else h.unk0 = 0; func_800CB408(&h); func_800CB4D4(&h); func_800CB4D4(&h); }
