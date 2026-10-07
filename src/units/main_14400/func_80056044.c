#include "common.h"

typedef unsigned short u16;
typedef short s16;

/* 16-byte spawn-script row of the 112-entry table D_80139B4C, defined
 * (initialized) in func_800560EC.c: wait is the frame delay after spawning
 * the row, -1 in the terminator row. */
typedef struct { u16 id; u16 variant; s16 x; s16 y; s16 z; s16 wait; s16 f0C; s16 f0E; } Spawn;

extern s32 D_80139B30;
s32 D_80139B34 = 0;
s32 D_80139B38 = 0;
extern Spawn D_80139B4C[];

void func_800560EC(s32 index);

void func_80056044(void)
{
    if (D_80139B30 == 1 && D_80139B4C[D_80139B38].wait != -1) {
        if (D_80139B34 <= 0) {
            do {
                func_800560EC(D_80139B38);
                D_80139B34 = D_80139B4C[D_80139B38].wait;
                D_80139B38++;
            } while (D_80139B34 == 0);
        }
        D_80139B34--;
    }
}
