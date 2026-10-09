#include "common.h"
typedef struct { unsigned char field_0, field_1, field_2, field_3; } Entry;
typedef struct { unsigned char field_0[3]; unsigned char field_3; unsigned char field_4[3]; unsigned char field_7; } Object;
extern Entry D_80143394[];
Entry *func_800B51D4(Object *self) { Entry *empty = 0; Entry *entry = D_80143394; s32 index = 0; do { if (entry->field_0 == self->field_7 && entry->field_1 == self->field_3) return entry; if (!entry->field_2) empty = entry; ++index; ++entry; } while (index < 40); return empty; }
