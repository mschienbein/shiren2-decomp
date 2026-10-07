#include "common.h"
extern signed char D_801546F0[][5];
typedef struct { char pad[0x34]; unsigned char cells[15]; } Board;
static inline s32 Board_columns(void){ return 5; }
s32 func_800D1A70(Board *b, s32 row, signed char *out){
    s32 ok = 1;
    s32 i, j;
    for (i = 0; i < Board_columns(); i++) {
        if (D_801546F0[row][i] != 0) {
            for (j = 2; j >= 0; j--) {
                if (b->cells[j + i * 3] != 0) break;
            }
            if (j >= 0) *out = j; else { *out = -2; ok = 0; }
        } else {
            *out = -1;
        }
        out++;
    }
    return ok;
}
