#include "common.h"
typedef struct { s32 field_00, field_04; unsigned char field_08[0x70]; s32 field_78; unsigned char field_7C[0xC0]; } Entry;
extern s32 D_801CA6D4;
extern Entry *D_801CA6DC;
s32 func_8012A3BC(s32 flags) { s32 i = 0; Entry *entry = D_801CA6DC; s32 count = 0; for (; i < D_801CA6D4; i++, entry++) { if (entry->field_04 && (entry->field_78 ? (flags & 1) : (flags & 2))) count++; } return count; }
