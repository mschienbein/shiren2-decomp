#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_800A3138(MapRect *p);
s32 func_800A315C(MapRect *p);
s32 func_800A3214(MapRect *p) { return func_800A3138(p) * func_800A315C(p); }
