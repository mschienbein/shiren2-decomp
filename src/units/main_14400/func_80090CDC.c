#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x18]; u8 channels[4]; } Color;

void func_80090CDC(Color *color, u8 channel, float value) {
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    if (channel == 3) {
        value = 1.0f - value;
    }
    color->channels[channel] = (u32)(value * 255.0f);
}
