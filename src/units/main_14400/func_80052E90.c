#include "common.h"

/* Fifteen 12-byte records; the reset fields start four bytes into each row. */
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

void func_80052E90(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        EntryState *entry = &D_801397E0[i].state;

        entry->state_08 = 0;
        entry->state_09 = 0x80;
        entry->key_04 = 0;
    }
}
