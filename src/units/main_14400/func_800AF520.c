#include "common.h"
extern char D_80148720[], D_801485F0[], D_80148650[], D_801485B0[], D_80148690[], D_80148470[];

/* 0x54-byte pointer table at 0x8015380C (original bytes, .rodata) */
char *const D_8015380C[21] = {
    0, D_80148720, D_801485F0, 0, 0, 0, D_80148650, D_801485B0, 0, D_80148690, D_80148470,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

char *const *func_800AF520(void) { return D_8015380C; }
