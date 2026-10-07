#include "common.h"
typedef struct { char pad[0x10]; unsigned char field_10; } Obj;
s32 func_8011DB64(void *self, s32 b) { Obj *a = self; if (b == 7) return a->field_10 != 0; if (b == 15) return a->field_10 == 0; return b == 7 || b == 11; }
