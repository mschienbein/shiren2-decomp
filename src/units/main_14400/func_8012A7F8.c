#include "common.h"

typedef unsigned char u8;
typedef struct Record_8012A7C4 Record_8012A7C4;

typedef struct Voice8012A7F8 {
    u8 pad_00[0x44];
    s32 key_44;
    u8 pad_48[0x7C - 0x48];
    Record_8012A7C4 *record_7C;
    u8 pad_80[0x13C - 0x80];
} Voice8012A7F8;

extern s32 D_801CA6D4;
extern Voice8012A7F8 *D_801CA6DC;

/* Return the record pointer of the first voice playing `key`, or null. */
Record_8012A7C4 *func_8012A7F8(s32 key) {
    Voice8012A7F8 *voice;
    s32 i;

    if (key == 0) {
        return 0;
    }
    voice = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, voice++) {
        if (voice->key_44 == key) {
            return voice->record_7C;
        }
    }
    return 0;
}
