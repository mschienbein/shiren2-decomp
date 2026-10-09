#include "common.h"
typedef struct EntryState {
    s32 key_04;
    unsigned char state_08;
    unsigned char state_09;
    unsigned short unknown_0A;
} EntryState;
typedef struct Entry12 {
    short id_00;
    unsigned short unknown_02;
    EntryState state;
} Entry12;
extern Entry12 D_801397E0[15];
Entry12 *func_80053034(short key){ s32 i; Entry12 *e = D_801397E0; for (i = 14; i != -1; i--, e++) { if (e->id_00 == key) return e; } return 0; }
