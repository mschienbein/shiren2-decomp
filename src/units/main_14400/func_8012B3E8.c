#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad[0x1C]; } Voice8012B3E8;
typedef struct {
    u8 pad0[0x10];
    s32 field_10;
    u8 pad14[8];
    u32 field_1C;
    u8 pad20[0x58];
    s32 field_78;
    u8 pad7C[0x22];
    short field_9E;
    u16 field_A0;
    u8 padA2[0xE];
    short field_B0;
    u8 padB2[9];
    u8 field_BB;
    u8 field_BC;
    u8 field_BD;
    u8 field_BE;
    u8 padBF[5];
    u8 field_C4;
} Chan8012B3E8;
extern Voice8012B3E8 *D_801CA6D8;
extern s32 D_801CA6E8;
extern u16 D_801CA6EC;
extern u16 D_801CA6EE;
void func_801301E0(Voice8012B3E8 *voice, short value, s32 a2);
void func_801300C0(Voice8012B3E8 *voice, u8 pan);
void func_8012B3E8(Chan8012B3E8 *ch, s32 index) {
    u32 value = (u32)(ch->field_BC * ch->field_C4 * ch->field_BB * ch->field_9E) >> 13;
    if (value > 0x7FFF) value = 0x7FFF;
    if (ch->field_78 == 0) {
        value *= D_801CA6EE;
    } else {
        value *= D_801CA6EC;
    }
    value >>= 15;
    if (ch->field_10 != -1) {
        value = value * ch->field_10 / ch->field_1C;
    }
    if (value != ch->field_A0) {
        ch->field_A0 = value;
        func_801301E0(&D_801CA6D8[index], value, D_801CA6E8);
    }
    value = ((ch->field_BD * ch->field_B0) >> 7) & 0x7F;
    if (value != ch->field_BE) {
        ch->field_BE = value;
        func_801300C0(&D_801CA6D8[index], value);
    }
}
