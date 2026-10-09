#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* Partial view: four colour channel bytes at 0x14. */
typedef struct Obj80090C20 {
    u8 pad_00[0x14];
    u8 channels_14[4];
} Obj80090C20;

/* Set colour channel `index` from a 0..1 intensity; channel 3 (alpha) is inverted. */
void func_80090C20(Obj80090C20 *obj, u8 index, f32 value)
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    if (index == 3) {
        value = 1.0f - value;
    }
    obj->channels_14[index] = value * 255.0f;
}
