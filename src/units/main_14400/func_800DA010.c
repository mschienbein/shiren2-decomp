#include "common.h"

extern const s32 D_80157FA8[];
extern const unsigned char D_801581B8[48];
typedef struct { short h0; short pad; const void *p4; } S;
S *func_800DA010(S *p) { p->p4 = D_80157FA8; p->h0 = 0x34; p->p4 = D_801581B8; return p; }
