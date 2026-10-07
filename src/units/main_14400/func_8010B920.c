#include "common.h"
typedef struct { unsigned char field_0, field_1; unsigned char *field_4, *field_8; } Entry;
extern unsigned char D_80147620[];
extern s32 func_8010BEC4(void *, unsigned char);
extern s32 func_800C5844(void *, unsigned char, unsigned char);
s32 func_8010B920(void *arg, unsigned char id, Entry *entry) { s32 count; while (entry->field_0 != id) { if (!entry->field_0) return 0; entry++; } count = (unsigned char)func_8010BEC4(arg, id); if (count > 0) { count--; if (entry->field_8) return (unsigned char)func_800C5844(D_80147620, entry->field_4[count], entry->field_8[count]); return entry->field_4[count]; } return entry->field_1; }
