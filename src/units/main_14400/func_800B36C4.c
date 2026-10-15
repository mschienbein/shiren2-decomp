#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos800B36C4;
/* func_800B33BC returns its output pointer (original 0x800B3690: daddu v0,s6,zero). */
void *func_800B33BC(void *out, MapRect *desc, u8 kind, s32 arg3);

void *func_800B36C4(void *out, Pos800B36C4 *pos, u8 kind, s32 arg3) {
    MapRect desc = func_800B3080(pos);

    func_800B33BC(out, &desc, kind, arg3);
    return out;
}
