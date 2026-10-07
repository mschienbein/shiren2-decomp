#include "common.h"
typedef float f32;
typedef struct { char pad0[0x30]; f32 value; char pad34[0x10]; s32 id; char pad48[0x48]; f32 base; char pad94[0xA8]; } Entry13C;
extern s32 D_801CA6D4;
extern Entry13C *D_801CA6DC;
s32 func_8012A5F0(s32 id, f32 delta){
    s32 i, count;
    Entry13C *e;
    if (id == 0) return 0;
    i = 0; count = 0;
    for (e = D_801CA6DC; i < D_801CA6D4; i++, e++) {
        if (e->id == id) { e->value = delta + e->base; count++; }
    }
    return count;
}
