#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x24]; const void *field_24; u8 pad_28[0x8C]; const void *field_B4; u8 pad_B8[0xC]; void *field_C4[4]; } Object;
extern const u8 D_8015C880[], D_8015C8A0[], D_80159130[], D_80159150[];
extern s32 func_800EE598(Object *);
extern void func_800CD468(void *);
extern void func_800CE6A0(void **, s32);
extern void func_800E016C(Object *, s32);
extern void func_800A3918(Object *);
void func_80109FDC(Object *object, s32 flags) { void **list; object->field_B4 = D_8015C880; object->field_24 = D_8015C8A0; func_800EE598(object); list = object->field_C4; func_800CD468(list); func_800CE6A0(list, 2); object->field_B4 = D_80159130; object->field_24 = D_80159150; func_800E016C(object, 0); if (flags & 1) func_800A3918(object); }
