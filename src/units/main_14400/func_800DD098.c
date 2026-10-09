#include "common.h"
typedef unsigned char u8;
/* Contained collection/item link at +8; func_800DA9DC reads its item pointer at +4.
 * +0x10 is a 4-byte byte record copied unaligned by func_800DCFF0/func_800DD0F4. */
typedef struct { void *collection; void *item; } Link;
typedef struct { u8 bytes[4]; } Word;
typedef struct { u8 field0; u8 field1; char pad2[6]; Link field8; Word field10; } Obj;
extern void func_800DA9DC(u8 *, Link *);
s32 func_800DD098(Obj *p, u8 *out) { *out++ = p->field1; func_800DA9DC(out++, &p->field8); *(Word *)out = p->field10; return 6; }
