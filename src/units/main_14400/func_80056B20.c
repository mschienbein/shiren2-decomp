#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef union { u32 word; struct { u8 r, g, b, a; } bytes; } Color;
typedef struct { u8 pad00[0x17]; u8 alpha17; Color color18; u8 pad1C[0x34]; s16 mode50; u8 pad52[6]; s16 duration58; s16 elapsed5A; } Obj;

void func_80056B20(Obj *obj)
{
    s16 time;
    s32 alpha;
    s32 fade;
    if (obj->duration58 == -1) return;
    time = obj->duration58 < obj->elapsed5A + 1 ?
        (unsigned short)obj->duration58 : (unsigned short)obj->elapsed5A + 1;
    obj->elapsed5A = time;
    alpha = (s32)((float)obj->elapsed5A / (float)obj->duration58 * 255.0f);
    fade = 255 - alpha;
    obj->color18.bytes.r = 255 - (u32)((float)(255 - fade) / 255.0f * 255.0f);
    obj->color18.bytes.g = 255 - (u32)((float)(255 - fade) / 255.0f * 255.0f);
    obj->color18.bytes.b = 255 - (u32)((float)(255 - fade) / 255.0f * 255.0f);
    obj->alpha17 = fade;
    if (!(obj->color18.word & 0xFFFFFF00) && !obj->alpha17) {
        obj->mode50 = 5;
        obj->elapsed5A = 0;
    }
}
