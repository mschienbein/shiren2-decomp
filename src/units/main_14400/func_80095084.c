#include "common.h"
typedef struct { char pad[0x14]; unsigned short field_14; char *field_18; } Obj;
extern void func_800950A8(void *, void *);
void func_80095084(Obj *a, char *b, void *c) { a->field_18 = b; a->field_14 = 0; func_800950A8(a, c); }
