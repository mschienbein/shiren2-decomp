#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad[0x3FC]; s8 room; u8 pad3FD[3]; s8 kind; } S;
typedef struct { u8 data[20]; } Room;
extern void *D_801476B8;
extern Room D_801431F0[];
s32 func_80049CB4(s32 id, ...);
s32 func_800A5B98(void *, Pos *);
s32 func_800BAA98(S *, Pos *);
s32 func_800BAAE0(S *, Pos *);
s32 func_800A31C8(Room *, Pos *);
void func_800A58FC(void *, Pos *);
void func_800BF944(S *s) {
    Pos pos;
    s32 n;
    Pos *pp;
    func_80049CB4(0x86, D_801476B8);
    n = 100;
    for (;;) {
        Room *room;
        if (--n == -1) break;
        if ((func_800A5B98(D_801476B8, &pos) ^ 1) != 0) continue;
        if (func_800BAA98(s, &pos)) continue;
        if (func_800BAAE0(s, &pos)) continue;
        if (s->kind == 11) {
            room = &D_801431F0[s->room];
            if ((func_800A31C8(room, &pos) ^ 1) != 0) continue;
        }
        func_800A58FC(D_801476B8, &pos);
        return;
    }
    pp = &pos;
    pp->x = 10;
    pp->y = 10;
    func_800A58FC(D_801476B8, &pos);
}
