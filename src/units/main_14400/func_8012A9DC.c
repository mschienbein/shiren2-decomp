#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[8];
    s32 value_08;
} Source;

/* Sound voice record (0x13C bytes) in the D_801CA6DC table. */
typedef struct {
    u8 pad0[0x44];
    s32 key_44;
    u8 pad48[0x74 - 0x48];
    Source *override_74;
    Source *source_78;
    u8 pad7C[0x13C - 0x7C];
} Voice8012A9DC;

extern s32 D_801CA6D4;
extern Voice8012A9DC *D_801CA6DC;

/* Return value_08 of the active source of the first voice playing `key`, or 0. */
s32 func_8012A9DC(s32 key) {
    s32 i;
    Voice8012A9DC *voice;

    if (key == 0) {
        return 0;
    }
    voice = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, voice++) {
        if (voice->key_44 == key) {
            if (voice->override_74 != 0) {
                return voice->override_74->value_08;
            }
            return voice->source_78->value_08;
        }
    }
    return 0;
}
