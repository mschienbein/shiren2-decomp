#include "common.h"

typedef struct { char pad[0x18]; short off; short pad1a; void (*fn)(void*, s32, void*); } VT;
typedef struct { char pad[0x18]; VT *vt; } O;
extern char D_8015D0CC[];
void func_800AF11C(void*, O*); void func_800CA4A4(O*, void*);
void func_8010CB78(char *a, O *b){ func_800AF11C(a, b); func_800CA4A4(b, D_8015D0CC); b->vt->fn((char*)b + b->vt->off, 2, a + 0xC); }
