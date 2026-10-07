#include "common.h"

typedef struct { char pad[0x24]; void *field24; } Object;
extern char D_8015B8F8[];
extern void func_800EFD28(Object *,s32),func_800A3918(Object *);
void func_80102DE8(Object *obj,s32 flags) { obj->field24=D_8015B8F8; func_800EFD28(obj,0); if (flags & 1) func_800A3918(obj); }
