#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x58]; void *field_58; u8 pad_5C[0x38]; s32 field_94; s32 field_98; } Object;
/* a1 is dereferenced at +0x1E and +0x0A by original func_800F40F0. */
extern s32 func_800F40F0(void *self, void *item, u8 *out);
s32 func_800F6298(Object *self, void *item, u8 *out) { *out = 0; if (!item) return 0; if (self->field_94) return func_800F40F0(self, item, out); if (self->field_98 && self->field_58 == item) return 2; return 0; }
