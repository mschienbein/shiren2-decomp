#include "common.h"
typedef short s16;
/* Item vtable: slot +0x4C (entry 9) is the pair action (self, source, target); its targets
 * (func_80117000, func_801181C0, ...) take the source and target units as pointers. */
typedef struct { char pad[0x48]; s16 offset48; s16 field4A; void (*call4C)(void *self, void *source, void *target); } Table;
typedef struct { s32 field0; s32 field4; Table *field8; } Obj;
/* Item vtable slot +0x44 (entry 8, (self, unit)): run this object's pair action on `unit` with no source. */
void func_80118190(Obj *p, void *unit) { Table *t = p->field8; t->call4C((char *)p + t->offset48, 0, unit); }
