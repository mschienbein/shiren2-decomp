#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x44];
    s32 key_44;
    u8 pad48[0xCB - 0x48];
    u8 volume_CB;
    u8 state_CC;
    u8 padCD[0x13C - 0xCD];
} Voice8012A6DC;

extern s32 D_801CA6D4;
extern Voice8012A6DC *D_801CA6DC;

s32 func_8012A6DC(s32 key, s32 volume) {
    s32 i;
    s32 count;
    Voice8012A6DC *voice;

    if (key == 0) {
        return 0;
    }
    if (volume < 0) {
        volume = 0;
    } else if (volume >= 0x80) {
        volume = 0x7F;
    }
    count = 0;
    voice = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, voice++) {
        if (voice->key_44 == key) {
            count++;
            voice->volume_CB = volume;
            voice->state_CC = 0xFF;
        }
    }
    return count;
}
