#include "common.h"
typedef unsigned char u8;
typedef struct { char pad0[0x4]; u8 unk4; char pad5[0x78 - 0x5]; } Entry;
/* func_8008D220 allocates among eight 0x78-byte slots at offset 0x100. */
typedef struct { char pad0[0x100]; Entry entries[8]; } Table;
extern s32 D_8013FEE0;
extern Table *D_8013FEE4;

void func_8008C528(s32 index, u8 value) { if (D_8013FEE0 != 0) { (D_8013FEE4->entries + index)->unk4 = value; } }
