#include "common.h"
typedef struct { unsigned char field_0[0x78]; short field_78; short field_7A; void (*field_7C)(void *, short); } VTable;
typedef struct { unsigned char field_0[0x1E]; unsigned char field_1E; unsigned char field_1F[5]; VTable *field_24; } Object;
/* Trap apply slot +0x54 supplies five pointers; self, actor, direction and
 * attacker are unused by this override. */
void func_8011DFE8(void *self, void *actor, void *target, void *direction, void *attacker) { Object *arg = target; if(arg->field_1E & 0x7C) arg->field_24->field_7C((char *)arg + arg->field_24->field_78,-1); }
