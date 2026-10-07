#include "common.h"
typedef short s16;
typedef struct { char pad0[0x44]; s32 id; char pad48[0x9E - 0x48]; s16 unk9E; char padA0[0x13C - 0xA0]; } Ent;
extern s32 D_801CA6D4;
extern Ent *D_801CA6DC;
s32 func_8012A534(s32 id, s32 value) {
    s32 i;
    s32 count;
    Ent *ent;
    if (id == 0) {
        return 0;
    }
    count = 0;
    ent = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, ent++) {
        if (ent->id == id) {
            ent->unk9E = value;
            count++;
        }
    }
    return count;
}
