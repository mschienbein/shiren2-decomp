#include "common.h"
typedef unsigned char u8;
typedef struct { s32 pad[3]; u8 *ptr; s32 cnt; } Stream;
void func_800839C4(Stream *);
u8 func_80083A10(Stream *s) {
    if (s->cnt <= 0) func_800839C4(s);
    s->cnt--;
    return *s->ptr++;
}
