#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad_00[0x74];
    void *field_74;
    void *field_78;
    u8 pad_7C[0x1C];
    short field_98;
    short pad_9A;
    short field_9C;
    u8 pad_9E[0xA];
    short field_A8;
    u8 pad_AA[0x92];
} Voice;
extern s32 D_801CA6E4;
extern s32 D_801CA6D4;
extern Voice *D_801CA6DC;

u8 *func_80129384(Voice *voice, u8 *command) {
    s32 rate = (*command++ * 0x6000 / 120) / D_801CA6E4;
    s32 scaled = (rate * voice->field_98) >> 7;
    s32 i;
    Voice *entry;
    if (voice->field_78 != 0) {
        voice->field_9C = rate;
    } else {
        entry = D_801CA6DC;
        for (i = 0; i < D_801CA6D4; i++, entry++) {
            if (entry->field_74 == voice->field_74) {
                entry->field_A8 = rate;
                entry->field_9C = scaled;
            }
        }
    }
    return command;
}
