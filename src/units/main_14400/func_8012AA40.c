#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x20];
    s32 value_20;
} Override8012AA40;

typedef struct {
    u8 pad0[0x14];
    s32 value_14;
} Source8012AA40;

/* Sound voice record (0x13C bytes) in the D_801CA6DC table. */
typedef struct {
    u8 pad0[0x44];
    s32 key_44;
    u8 pad48[0x74 - 0x48];
    Override8012AA40 *override_74;
    Source8012AA40 *source_78;
    u8 pad7C[0x13C - 0x7C];
} Voice8012AA40;

extern s32 D_801CA6D4;
extern Voice8012AA40 *D_801CA6DC;

s32 func_8012AA40(s32 key) {
    s32 i;
    Voice8012AA40 *voice;

    if (key == 0) {
        return 0;
    }
    voice = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, voice++) {
        if (voice->key_44 == key) {
            if (voice->override_74 != 0) {
                return voice->override_74->value_20;
            }
            return voice->source_78->value_14;
        }
    }
    return 0;
}
