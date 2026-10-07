#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 field_0; u8 field_1; u8 pad2[0xA]; float field_C; u8 pad10[0x68]; } Channel;
typedef struct { u8 pad0[0x100]; Channel channels[8]; } Bank;
extern s32 D_8013FEE0;
extern Bank *D_8013FEE4;
float func_8008CE34(Channel *ch);
s32 func_8008CE58(Channel *ch);
s32 func_8008C1C8(s32 index, float delta) {
    Channel *ch;
    float target;
    float value;
    s32 result = 0;

    if (D_8013FEE0 == 0 || (ch = &D_8013FEE4->channels[index])->field_0 == 0) {
        return -1;
    }
    if (ch->field_1 != 0) {
        target = func_8008CE34(ch);
        if (ch->field_C == target && delta != 0.0f) {
            ch->field_1 = 0;
            result = 1;
        } else if (ch->field_C == 0.0f) {
            ch->field_C = 1.0f;
        } else {
            value = ch->field_C + delta;
            ch->field_C = value;
            if (target < value) {
                ch->field_C = target;
            }
        }
        if (func_8008CE58(ch) != 0) {
            result |= 2;
        }
    } else {
        result = 4;
    }
    return result;
}
