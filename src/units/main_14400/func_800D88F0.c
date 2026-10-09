#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0xC0]; u8 *field_C0; u8 *field_C4; u8 pad_C8[4]; void *field_CC; s32 field_D0; } Object;
extern void func_800D788C(u8 *);
extern void *func_8011422C(u8 *);
extern void func_800CD364(void *, void *);
extern void *func_800D7A24(u8, u8);
extern s32 func_800CD538(void *, void *);
void func_800D88F0(Object *object) { u8 *item; switch (object->field_D0) { case 1: func_800D788C(object->field_C4); func_800CD364(func_8011422C(object->field_C0), object->field_C4); func_800CD364(object->field_CC, object->field_C0); break; case 2: item = func_800D7A24(object->field_C4[0xD], object->field_C4[0xE]); func_800D788C(object->field_C4); func_800CD364(func_8011422C(object->field_C0), object->field_C4); func_800CD538(func_8011422C(object->field_C0), item); object->field_C4 = item; break; } object->field_D0 = 0; }
