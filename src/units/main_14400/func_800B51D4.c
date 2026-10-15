#include "common.h"
typedef struct { unsigned char field_0, field_1, field_2, field_3; } Entry;
typedef struct { s32 x; s32 y; } Object;
extern Entry D_80143394[];
/* Coordinates occupy full words; the ROM compares their low bytes at +3/+7. */
Entry *func_800B51D4(Object *self) { Entry *empty = 0; Entry *entry = D_80143394; s32 index = 0; do { if (entry->field_0 == (unsigned char)self->y && entry->field_1 == (unsigned char)self->x) return entry; if (!entry->field_2) empty = entry; ++index; ++entry; } while (index < 40); return empty; }
