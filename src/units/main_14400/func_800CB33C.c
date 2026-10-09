#include "common.h"
/* Stream object: +0x14 holds the failing tag/error text pointer (null when clean). */
typedef struct { unsigned char field_0[0x14]; const char *field_14; } Context;
typedef struct { s32 field_0; Context *field_4; } Object;
extern Context *func_80044678(s32, s32, s32 *);
extern void func_800CB470(Object *);
extern void func_800CB3C8(Object *);
s32 func_800CB33C(Object *self) { s32 status; self->field_4 = func_80044678(3, 0, &status); switch (status) { /* ODD_C: load status 0 needs no follow-up; the label shapes codegen: without it 23 words differ (128 vs 140 bytes). */ case 0: break; case 3: func_800CB470(self); if (!self->field_4->field_14) break; status = 2; case 1: case 2: func_800CB3C8(self); break; } self->field_4->field_14 = 0; return status; }
