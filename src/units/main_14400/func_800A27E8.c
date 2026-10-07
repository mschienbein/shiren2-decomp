#include "common.h"
typedef unsigned char u8;
typedef struct { s32 a, b; } Tmp;
extern Tmp *func_800A256C(Tmp *, Tmp *, Tmp *);
extern u8 *func_800A2228(u8 *, Tmp *);
void *func_800A27E8(void *self, void *a, void *b){ Tmp t; func_800A256C(&t, b, a); func_800A2228(self, &t); return self; }
