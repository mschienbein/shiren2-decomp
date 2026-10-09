#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef short s16;
typedef struct { s32 unk0, unk4; } Pair;
/* Actor status slot +0x94: func_800F212C / func_800E115C, with zero this-adjustment. */
typedef struct { char unk0[0x90]; s16 unk90, unk92; s32 (*unk94)(void *, s32, s32, u8, s32); } Dispatch;
typedef struct { char unk0[0x24]; Dispatch *unk24; } State;
/* Game mode flags at 0x80142F20 (same view as func_8011C490): mode in the top three bits. */


extern u32 D_8013960C;
extern s32 func_800B274C(s32);
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern s32 func_800A8FC8(s32 *, s32);
extern State *func_800A910C(s32 *);
extern s32 func_800A6E90(State *);
/* Slot +0x44 supplies the receiver and item, which this implementation ignores. */
void func_8011CE90(void *self, Pair *arg1, void *item) {
    Pair position;
    s32 iterator;
    Pair *current = &position;
    s32 is_mode1;
    current->unk0 = arg1->unk0;
    current->unk4 = arg1->unk4;
    is_mode1 = (D_80142F18.mode & 0xE0) == 32;
    if (is_mode1) {
        if (func_800B274C(0x80)) {
            func_80049CB4(0xFD, current);
            func_80049CB4(0xDB);
            func_800498E4(0xDC);
            iterator = 0;
            D_8013960C <<= 1;
            for (;;) {
                s32 valid = func_800A8FC8(&iterator, 0x7C);
                State *state;
                if (!valid) break;
                state = func_800A910C(&iterator);
                if (func_800A6E90(state)) {
                    Dispatch *dispatch = state->unk24;
                    dispatch->unk94((char *)state + dispatch->unk90, 0, 0xD, 0xFF, 0);
                }
            }
            D_8013960C >>= 1;
        } else {
            func_80049CB4(0x132);
            func_800498E4(0x223);
        }
    } else {
        func_80049CB4(0x11D, current);
        func_800498E4(0x225);
    }
}
