#include "common.h"

typedef struct { char pad[0x10]; unsigned char x10; unsigned char x11; char pad2[0x3CA]; s32 x3DC; } S;
void func_800B9684(S *, s32);
void func_800B98F4(S *, unsigned char, unsigned char);
void func_800B95DC(S *s) {
    s32 i, j, k;
    for (i = 0; i < s->x3DC; i++) func_800B9684(s, i);
    for (j = 1;; j++) {
        s32 n = s->x10;
        if (n < j) break;
        for (k = 1; k <= s->x11; k++) func_800B98F4(s, j, k);
    }
}
