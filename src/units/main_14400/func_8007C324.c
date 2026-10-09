#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

/* 0xB0-byte unit record of the D_801DEAB4 table: kind at +2 (-1 = empty),
 * facing direction (0..7) at +0x4A. */
typedef struct ActorRecord {
    s16 field_00;
    s16 active_02;
    u8 pad_04[0x46];
    s16 direction_4A;
    u8 pad_4C[0x64];
} ActorRecord;

extern s32 D_8013D8CC;
extern ActorRecord D_801DEAB4[30];

/* Interface note: this caller passes both values un-narrowed (daddu a0,s0 at
 * 0x8007C36C; a1 = full-width direction at 0x8007C3DC). func_80042944 narrows
 * its id itself (andi a0,0xFF at 0x80042950) and func_8007C1E0 only stores the
 * halfword, so both parameters are full-width s32 (canonical u8/s16 to be
 * corrected; see results.json). */
extern s32 func_80042944(s32 id);
extern s32 func_8007C1E0(s32 index, s32 direction);

/* Turns every placed unit one step (of 8 directions) towards the direction
 * func_80042944 reports, taking the shorter way round.
 * Returns -1 while disabled, 0 otherwise. */
s32 func_8007C324(void)
{
    s32 i;
    ActorRecord *record;
    s32 target, current;

    if (!D_8013D8CC)
        return -1;
    for (i = 0; i < 30; i++) {
        record = &D_801DEAB4[i];
        if (record->active_02 == -1)
            continue;
        target = func_80042944(i);
        if (target == -1)
            continue;
        current = record->direction_4A;
        if (current == target)
            continue;
        if (current < target) {
            if (target - current < 5) {
                current++;
                target = 0;
                if (current < 8)
                    target = current;
            } else {
                target = current - 1;
                if (target < 0)
                    target = 7;
            }
        } else {
            if (current - target >= 5) {
                target = 0;
                current++;
                if (current < 8)
                    target = current;
            } else {
                target = current - 1;
                if (target < 0)
                    target = 7;
            }
        }
        func_8007C1E0(i, target);
    }
    return 0;
}
