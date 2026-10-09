#include "common.h"
typedef struct { unsigned char pad_0[8]; short field_8; short field_A; void (*field_C)(void *, s32); } Methods;
typedef struct { unsigned char pad_0[8]; Methods *field_8; } Mon;
typedef struct Pos Pos;
extern s32 func_80116230(Mon *, void *);
extern s32 func_80049CB4(s32, ...);
extern void func_800AD868(Pos *);
s32 func_8011587C(Mon *object, Pos *position) { s32 result; if (func_80116230(object, position)) { func_80049CB4(0x107, position); func_800AD868(position); if (object) object->field_8->field_C((unsigned char *)object + object->field_8->field_8, 3); result = 1; } else result = 0; return result; }
