#include "common.h"
/* 12-byte fade record: only its address is used here. */
typedef struct { s32 field_0; unsigned char pad4[8]; } Entry;
extern Entry D_801616D0[], D_801616E8;
extern unsigned char D_801398A0;
extern unsigned char func_800535C8(Entry *);
extern void func_800536F0(Entry *);
extern void func_80053808(void);
static inline void process(Entry *entry) { if (func_800535C8(entry)) func_800536F0(entry); }
void func_800534D4(void)
{
    Entry *entry = D_801616D0;
    Entry *special = &D_801616E8;
    s32 i;
    process(special);
    process(special);
    for (i = 1; i != -1; i--, entry++) process(entry);
    if (D_801398A0) func_80053808();
}
