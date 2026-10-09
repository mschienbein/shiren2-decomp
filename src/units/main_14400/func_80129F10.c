#include "common.h"
typedef struct Track {
    s32 field_00; unsigned char *field_04; s32 field_08, field_0c;
    unsigned char field_10[0x24]; void *field_34, *field_38;
    s32 field_3c, field_40, field_44;
    unsigned char field_48[0x2c]; void *field_74;
    unsigned char field_78[0x22]; unsigned short field_9a;
    unsigned char field_9c[0xe]; unsigned short field_aa, field_ac;
    unsigned char field_ae[9]; unsigned char field_b7;
    unsigned char field_b8[3]; unsigned char field_bb;
    unsigned char field_bc[0x16]; unsigned char field_d2, field_d3;
    unsigned char field_d4[0x68];
} Track;
extern unsigned char *(*D_801487D0[])(Track *, unsigned char *);
extern s32 D_801CA6D4;
extern Track *D_801CA6DC;
extern s32 func_8012C4B0(void *);
extern void func_8012A870(s32), func_8012BA20(Track *), func_8012BADC(Track *);
s32 func_80129F10(void *owner, s32 marker) {
    s32 id = func_8012C4B0(owner);
    s32 i;
    Track *track = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, track++) {
        if (track->field_44 == id && track->field_74 == owner && track->field_04) {
            do {
                unsigned char *cursor = track->field_04;
                unsigned char value = *cursor;
                if ((signed char)*cursor < 0) {
                    if (value == 0xab && cursor[1] == marker) break;
                    track->field_04 = D_801487D0[value & 0x7f](track, cursor + 1);
                } else {
                    unsigned short duration;
                    track->field_04 = cursor + 1;
                    if (track->field_d2) {
                        track->field_bb = *track->field_04++;
                        if ((signed char)track->field_bb < 0) {
                            track->field_bb &= 0x7f;
                            track->field_d2 = 0;
                            track->field_d3 = track->field_bb;
                        }
                    } else {
                        track->field_bb = track->field_d3;
                    }
                    duration = track->field_ac;
                    if (duration && !track->field_b7) {
                        track->field_9a = duration;
                    } else {
                        unsigned char duration_byte;
                        track->field_b7 = 0;
                        duration_byte = *track->field_04++;
                        if ((signed char)duration_byte >= 0) track->field_9a = duration_byte & 0xff;
                        else track->field_9a = *track->field_04++ + ((duration_byte & 0x7f) << 8);
                    }
                    track->field_0c += track->field_9a << 8;
                }
            } while (track->field_04);
            track->field_3c = track->field_0c;
            if (track->field_04) {
                unsigned char *cursor = track->field_04;
                s32 duration = cursor[2];
                unsigned char *next = cursor + 3;
                if (duration >= 0x80) {
                    duration &= 0x7f;
                    duration <<= 8;
                    duration |= *next++;
                }
                track->field_9a = duration;
                track->field_aa = 0;
                track->field_04 = next;
                track->field_0c -= duration << 8;
            }
            track->field_40 = track->field_0c;
            if (track->field_38) func_8012BA20(track);
            if (track->field_34) func_8012BADC(track);
        }
    }
    func_8012A870(id);
    return id;
}
