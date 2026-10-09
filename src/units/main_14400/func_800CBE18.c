#include "common.h"

typedef unsigned char u8;

typedef struct State800CBE18 {
    s32 total_00;
    u8 pad_04[0xC];
    u8 level_10;
} State800CBE18;

extern State800CBE18 *D_80147F44;
extern const s32 D_801541E4[];

/* True while total_00 + amount stays below the current level's limit. */
s32 func_800CBE18(s32 amount) {
    return D_80147F44->total_00 + amount < D_801541E4[D_80147F44->level_10];
}
