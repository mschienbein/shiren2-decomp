#include "common.h"
typedef struct { unsigned char field_00[0x18]; short field_18; void (*field_1C)(void *, s32, void *); } VTable;
typedef struct { unsigned char field_00[0x18]; VTable *field_18; } Object;
extern char D_8015D724[];
extern void func_800AF11C(void *owner, void *stream);
extern void func_800CA4A4(Object *, void *);
void func_801133B0(char *owner, Object *object) { func_800AF11C(owner, object); func_800CA4A4(object, D_8015D724); object->field_18->field_1C((char *)object + object->field_18->field_18, 1, owner + 0xC); }
