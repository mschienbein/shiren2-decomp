#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { u8 pad_0[0x14]; u8 flags[8][4]; u8 counts[5][3]; u8 scores[5][4]; s8 records[5][4][5]; } Board;
extern const u8 D_8015488C[8], D_80154894[8]; /* single-bit set/clear masks */
extern s32 func_800D1A70(Board *, s32, s8 *);
extern s32 func_800D1488(void *, u8);
extern s32 func_800D18A8(s8 *);
static inline void put_bit(u8 *bits, s32 bit, s32 on) {
    if (on) bits[bit >> 3] |= D_8015488C[bit & 7];
    else bits[bit >> 3] &= D_80154894[bit & 7];
}

void func_800D1344(Board *board, u8 row) {
    s8 values[5];
    s32 slot, i;
    func_800D1A70(board, row, values);
    slot = func_800D1488(board, row);
    if (slot >= 0) {
        i = 0;
        for (;;) {
            s8 value;
            if (i >= 5) break;
            board->records[row][slot][i] = value = values[i];
            if (value >= 0) {
                s32 on;
                board->counts[i][value]--;
                on = value == 2;
                put_bit(&board->flags[row][slot], i, on);
            }
            i++;
        }
        board->scores[row][slot] = func_800D18A8(values);
    }
}
