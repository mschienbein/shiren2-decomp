#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef float f32;

/* Animation frame; id -1 marks a jump to frame `arg` (0 when arg is -1),
 * otherwise `arg` is the frame duration. */
typedef struct {
    s8 id;
    s8 arg;
    s16 value;
} AnimFrame;

typedef struct {
    s32 count;
    AnimFrame *frames;
} Anim;

typedef struct {
    u8 pad0[0x7];
    u8 state7;
    u8 pad8[0xC];
    f32 value14;
    u8 pad18[0x26];
    u8 id3E;
    u8 pad3F[0x3];
    u8 frame42;
    s8 timer43;
} Animator;

/* Advance the animator by dt ticks and latch the next frame when its timer expires. */
void func_80074D88(Animator *obj, Anim *anim, s32 dt) {
    u8 index;

    if (obj->state7 != 1) {
        return;
    }
    if (obj->timer43 > 0) {
        if (dt < obj->timer43) {
            obj->timer43 -= dt;
        } else {
            obj->timer43 = 0;
        }
    }
    if (obj->timer43 != 0) {
        return;
    }
    index = obj->frame42;
    if (anim->frames[index].id == -1 || index >= anim->count) {
        if (anim->frames[index].arg == -1) {
            obj->frame42 = 0;
        } else {
            obj->frame42 = anim->frames[index].arg;
        }
    }
    obj->value14 = anim->frames[obj->frame42].value;
    obj->id3E = anim->frames[obj->frame42].id;
    obj->timer43 = anim->frames[obj->frame42].arg;
    obj->frame42++;
}
