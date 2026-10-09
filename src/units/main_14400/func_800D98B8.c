#include "common.h"
typedef unsigned char u8;
/* Original slot return is word-sized: a byte-return declaration adds an absent andi. */
typedef struct { u8 pad_0[0x20]; short field_20; short field_22; s32 (*field_24)(void *); } Methods;
/* Complete 0x14-byte list header, including the count read by slot +0x24. */
typedef struct { s32 field_0; Methods *field_4; void *records_8; s32 capacity_C, count_10; } List;
typedef struct { u8 field_0, field_1; u8 pad_2[0xAE]; List field_B0; } Object;
extern void func_800DDB64(Object *, u8 *, s32);
s32 func_800D98B8(Object *object, u8 *cursor) { List *list = &object->field_B0; s32 count = list->field_4->field_24((u8 *)list + list->field_4->field_20); *cursor++ = object->field_1; *cursor++ = count + 1; func_800DDB64(object, cursor, count); return count + 3; }
