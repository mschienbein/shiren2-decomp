#include "common.h"
typedef struct { s32 field_00; unsigned char field_04[0x60]; } Entry;
extern char D_8014C960[], D_8014C974[], D_8014C984[];
extern void *D_801A71C8, *D_801A71E8;
extern Entry *D_801A7224;
extern s32 D_801A71C0;
extern void *func_8006A8D8(char *, u32);
static inline void clear_entries(Entry *entries, s32 value) { s32 i; for (i = 27; i >= 0; i--) entries[i].field_00 = value; }
void func_800725E0(void) { Entry *entries; D_801A71C8 = func_8006A8D8(D_8014C960, 0x2000); D_801A71E8 = func_8006A8D8(D_8014C974, 0x1B0); entries = func_8006A8D8(D_8014C984, 0xAF0); D_801A7224 = entries; clear_entries(entries, -1); D_801A71C0 = 1; }
