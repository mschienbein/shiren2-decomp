#include "common.h"
extern void func_80048728(void *);
/* Text window subobject (func_800486A4 view): handle at +0xC. */
typedef struct { unsigned char fields_00[0xC]; s32 handle_0C; } Window;
typedef struct { unsigned char pad0[0x3E8]; Window field3E8; } Object;
void func_800989AC(Object *object) { func_80048728(&object->field3E8); }
