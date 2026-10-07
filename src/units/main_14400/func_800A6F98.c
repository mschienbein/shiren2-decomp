#include "common.h"
/* Entity slot +0x54: s32 (*)(void *self, s32 mode). */
typedef struct { unsigned char field_00[0x50]; short field_50; s32 (*field_54)(void *self, s32 mode); } VTable;
typedef struct { unsigned char field_00[0x24]; VTable *field_24; } Object;
extern char D_80147620[];
extern s32 func_800C587C(void *, unsigned char);
s32 func_800A6F98(Object *object, s32 mode) { return func_800C587C(D_80147620, (unsigned char)object->field_24->field_54((char *)object + object->field_24->field_50, mode)); }
