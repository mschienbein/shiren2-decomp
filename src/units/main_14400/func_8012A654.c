#include "common.h"
typedef struct { char pad[0x44]; s32 unk44; char pad48[0x98 - 0x48]; unsigned short unk98; char pad9A[2]; unsigned short unk9C; char pad9E[0xA8 - 0x9E]; unsigned short unkA8; char padAA[0x13C - 0xAA]; } E;
extern s32 D_801CA6D4;
extern E *D_801CA6DC;
s32 func_8012A654(s32 id, s32 vol) {
    s32 i, n;
    E *e;
    if (id == 0) return 0;
    if (vol <= 0) vol = 1;
    else if (vol > 0x100) vol = 0x100;
    n = 0;
    e = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, e++) {
        if (e->unk44 == id) {
            n++;
            e->unk98 = vol;
            e->unk9C = (e->unkA8 * vol) >> 7;
        }
    }
    return n;
}
