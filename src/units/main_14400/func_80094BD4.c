#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 start; s32 end; } Range;
/* Parameter-only prefix; func_80094BC8 owns initialization of the remaining fields. */
typedef struct { Range *f_0; u8 pad[3]; s8 f_7; } S;
void func_80094BC8(S *s);
void func_80094BD4(S *s, Range *v) { s->f_0 = v; func_80094BC8(s); s->f_7 = -1; }
