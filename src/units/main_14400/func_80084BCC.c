#include "common.h"
typedef struct { s32 field_0; char pad[0x70]; } Entry;
extern Entry D_801BA380[];
unsigned short func_80084BCC(void) { s32 i = 0; Entry *p = D_801BA380; for (; i < 0xAE; i++, p++) { if (!p->field_0) return i; } return 0xAE; }
