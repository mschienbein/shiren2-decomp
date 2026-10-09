#include "common.h"
typedef struct { char pad[0x1C]; void *field_1c; } Obj;
extern char D_8015F0E0[];
extern Obj *func_80111E08(Obj *, void *, void *, void *, unsigned char *);
Obj *func_8011F010(Obj *p, char *a, void *b) { func_80111E08(p, a, b, a, (unsigned char *)a + 8); p->field_1c = D_8015F0E0; return p; }
