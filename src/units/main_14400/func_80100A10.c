#include "common.h"
typedef struct { unsigned char field_00[0x10]; short field_10; s32 (*field_14)(void *, void *, s32, void *); } VTable;
typedef struct { unsigned char field_00[0x8A]; unsigned char field_8A; } Child;
typedef struct { unsigned char field_00[0xC]; VTable *field_0C; Child *field_10; } Object;
extern s32 func_800A58B8(void *);
extern u32 func_800B1C6C(void *);
extern void func_800A7B18(void *, void *, s32, s32);
static inline unsigned char nonzero(u32 flags) { return flags != 0; }
void func_80100A10(Object *object, void *a, s32 b, void *target) { s32 special = 0; if (func_800A58B8(target) == 1) { s32 flags = func_800B1C6C(target) & 0x2000; special = nonzero(flags); } if (special) func_800A7B18(target, a, object->field_10->field_8A, 2); else object->field_0C->field_14((char *)object + object->field_0C->field_10, a, b, target); }
