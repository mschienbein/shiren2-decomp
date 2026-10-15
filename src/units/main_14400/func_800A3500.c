#include "common.h"
#include "map_rect.h"
void func_800A28A0(MapPos *);
void func_800A3500(MapRect *p) { func_800A28A0(&p->min); func_800A28A0(&p->max); }
