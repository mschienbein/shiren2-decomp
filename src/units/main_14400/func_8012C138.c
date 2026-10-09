#include "common.h"

typedef unsigned char u8;

typedef struct Def8012C138 {
    u8 pad_00[0xC];
    u8 **values_0C;
} Def8012C138;

/* 0x13C-byte voice record (see func_8012C330 / func_8012A1DC). */
typedef struct Voice8012C138 {
    u8 pad_00[0x4];
    u8 *active_04;
    u8 pad_08[0x48 - 0x8];
    s32 priority_48;
    u8 pad_4C[0x74 - 0x4C];
    Def8012C138 *owner_74;
    Def8012C138 *def_78;
    u8 pad_7C[0x80 - 0x7C];
    u8 *value_80;
    u8 pad_84[0x13C - 0x84];
} Voice8012C138;

extern s32 D_801CA6D4;
extern Voice8012C138 *D_801CA6DC;
extern Voice8012C138 *D_801CA6E0;

/* Pick a voice index for `def`: a free reserved voice (slot < 0), a free voice, the
 * lowest-priority busy voice, then a voice already playing this sound. */
s32 func_8012C138(Def8012C138 *def, s32 slot) {
    Voice8012C138 *voice;
    s32 i;
    s32 best;
    s32 bestIndex;

    if (slot < 0) {
        for (i = 0, voice = D_801CA6DC; i < 4; i++, voice++) {
            if (voice->active_04 == 0) {
                return i;
            }
        }
    }
    for (i = 4, voice = D_801CA6E0; i < D_801CA6D4; i++, voice++) {
        if (voice->active_04 == 0) {
            return i;
        }
    }
    i = 4;
    best = 0x7FFFFFFF;
    bestIndex = 3;
    for (voice = D_801CA6E0; i < D_801CA6D4; i++, voice++) {
        if (voice->def_78 != 0 && voice->priority_48 <= best) {
            best = voice->priority_48;
            bestIndex = i;
        }
    }
    if (bestIndex >= 4) {
        return bestIndex;
    }
    /* The original never advances `voice` in this scan. */
    for (i = 4, voice = D_801CA6E0; i < D_801CA6D4; i++) {
        if (voice->def_78 == 0 && voice->owner_74 != def) {
            return i;
        }
    }
    for (i = 4, voice = D_801CA6E0; i < D_801CA6D4; i++, voice++) {
        if (voice->owner_74 == def && def->values_0C[slot] == voice->value_80) {
            return i;
        }
    }
    return slot % (D_801CA6D4 - 4) + 4;
}
