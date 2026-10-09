#include "common.h"

typedef struct VTable VTable;
extern VTable D_8015E890;
typedef struct { s32 x0, x4; VTable *vt8; s32 xC; s32 x10; s32 x14; } S;
S *func_80112D20(S *p, s32 size);
S *func_8011B930(S *p) { func_80112D20(p, 0x24); p->vt8 = &D_8015E890; p->x10 = 0; p->x14 = 0; return p; }