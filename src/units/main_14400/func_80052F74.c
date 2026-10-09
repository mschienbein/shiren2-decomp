#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

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
extern s16 D_801616B0[15];
s32 func_8012A4E4(s32 id);

/* Fills D_801616B0 with the ids of the active D_801397E0 entries, -1 terminated/padded. */
s16 *func_80052F74(void) {
    Entry12 *entry = D_801397E0;
    s16 *out = D_801616B0;
    s32 i;

    for (i = 14; i != -1; i--) {
        *out++ = -1;
    }
    out = D_801616B0;
    for (i = 14; i != -1; i--, entry++) {
        if (func_8012A4E4(entry->state.key_04) != 0) {
            *out++ = (u16)entry->id_00;
        }
    }
    return D_801616B0;
}
