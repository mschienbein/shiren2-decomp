#include "common.h"
typedef struct { char pad[0x28]; short field_28; void (*field_2C)(void *, s32, void *); } VTable;
typedef struct { char pad[0x18]; VTable *field_18; } Obj;
extern char D_8015D644[];
extern void func_8010CBD4(void *, Obj *), func_800CA4E8(Obj *, char *);
void func_80112710(char *a, Obj *b) { func_8010CBD4(a, b); func_800CA4E8(b, D_8015D644); b->field_18->field_2C((char *)b + b->field_18->field_28, 8, a + 0x10); }
