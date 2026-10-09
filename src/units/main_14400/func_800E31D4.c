#include "common.h"
typedef struct { unsigned char pad_0[0x90]; short field_90; short field_92; s32 (*field_94)(void *, s32, s32, unsigned char, s32); } Methods;
typedef struct { unsigned char pad_0[0x24]; Methods *field_24; } Object;
/* D_8015C5D0 + 0x94 points to func_800F212C, which returns a status. */
s32 func_800E31D4(Object *object) { return object->field_24->field_94((unsigned char *)object + object->field_24->field_90, 0, 1, 0xFE, 0); }
