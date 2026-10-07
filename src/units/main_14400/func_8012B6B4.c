#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef float f32;
typedef struct {
    u8 pad0[0xC];
    u32 now;
    u8 pad10[0x30];
    u32 start;
    u8 pad44[0x10];
    u32 releaseTime;
    f32 attackSlope;
    f32 decaySlope;
    f32 releaseSlope;
    u32 rate;
    u8 pad68[0x57];
    u8 field_BF;
    u8 attackLevel;
    u8 peakLevel;
    u8 sustainLevel;
    u8 state;
    u8 level;
    u8 field_C5;
    u8 attackTime;
    u8 decayTime;
    u8 releaseLength;
    u8 padC9[0x4];
    u8 releaseLevel;
} Env8012B6B4;
void func_8012B6B4(Env8012B6B4 *env) {
    s32 t;
    if ((s32)(env->releaseTime - env->now) < 0 && env->state < 4) {
        env->state = 4;
        env->field_C5 = 1;
        env->releaseLevel = env->level;
    }
    env->field_C5 = env->field_BF;
    switch (env->state) {
    case 1:
        t = (((env->now - env->start) >> 8) * env->rate) >> 10;
        if (t < env->attackTime) {
            env->level = env->attackLevel + (s32)(env->attackSlope * t);
        } else {
            env->state = 2;
            env->level = env->peakLevel;
        }
        break;
    case 2:
        t = ((((env->now - env->start) >> 8) - env->attackTime) * env->rate) >> 10;
        if (t < env->decayTime) {
            env->level = env->peakLevel + (s32)(env->decaySlope * t);
        } else {
            env->state = 3;
            env->level = env->sustainLevel;
        }
        break;
    case 3:
        break;
    case 4:
        t = (((env->now - env->releaseTime) >> 8) * env->rate) >> 10;
        if (t < env->releaseLength) {
            env->level = env->releaseLevel - (s32)(env->releaseSlope * t * env->releaseLevel);
        } else {
            env->state = 5;
            env->level = 0;
        }
        break;
    }
}
