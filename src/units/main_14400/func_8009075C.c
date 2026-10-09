#include "common.h"
typedef struct { unsigned char pad_0[0x18]; s32 field_18; void **field_1C; } Object;
/* 800905DC forwards its first argument to the stream reader 8008DF04. */
extern void *func_800905DC(void *, s32);
s32 func_8009075C(void *stream, s32 size, Object *object) { s32 result = 0; void *entry = func_800905DC(stream, size); if (!entry) result = -1; else object->field_1C[object->field_18++] = entry; return result; }
