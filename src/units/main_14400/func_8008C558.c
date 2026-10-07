#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[5]; u8 field5; u8 pad6[0x78 - 6]; } Entry;
/* Eight 0x78-byte channels follow the bank's 0x100-byte allocation header. */
typedef struct { u8 pad0[0x100]; Entry entries[8]; } Bank;
extern s32 D_8013FEE0;
extern Bank *D_8013FEE4;
void func_8008C558(s32 i, s32 v) { if (D_8013FEE0 != 0) (D_8013FEE4->entries + i)->field5 = v; }
