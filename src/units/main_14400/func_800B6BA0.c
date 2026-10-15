#include "map_rect.h"
typedef unsigned short u16;
typedef struct { MapRect bounds; unsigned char counts[4]; } Room800B6BA0;
extern char D_80147620[];
extern s32 func_800B68B0(Room800B6BA0 *);
extern MapPos *func_800A33DC(MapPos *, MapRect *);
extern u16 func_800C58DC(void *, u16);
extern MapPos *func_800B6A98(MapPos *, Room800B6BA0 *, s32);
void *func_800B6BA0(MapPos *out, Room800B6BA0 *in) { s32 index = func_800B68B0(in); if (!index) { func_800A33DC(out, &in->bounds); } else { u16 value = func_800C58DC(D_80147620, index - 1); func_800B6A98(out, in, value); } return out; }
