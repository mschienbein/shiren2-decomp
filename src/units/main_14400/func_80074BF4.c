#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef float f32;

typedef struct {
    u8 id;
    u8 next;
    s16 duration;
} AnimFrame;
typedef struct {
    u32 count;
    AnimFrame *frames;
} AnimTable;
typedef struct {
    u8 pad0[3];
    u8 animIndex;
} ActorInfo;
typedef struct {
    u8 pad0[2];
    s16 type;
    u8 pad4;
    s8 state;
    u8 variant;
    u8 pad7[2];
    u8 sub;
    u8 padA[0xA];
    f32 timer;
    u8 pad18[0x26];
    u8 frameId;
    u8 pad3F[3];
    u8 frame;
    u8 frameNext;
    u8 pad44[0x6C];
} Actor;
extern Actor D_801DEAB4[];
extern AnimTable D_8014D1B4[];
ActorInfo **func_80074784(s32, s32);
u32 func_8002A9B0(void);
void func_80074BF4(s32 index, s32 mode, s32 randomize) {
    Actor *actor = &D_801DEAB4[index];
    ActorInfo **info = func_80074784(actor->type, actor->variant);
    s32 anim;
    AnimTable *table;

    if (actor->type == 0x17) {
        switch (actor->sub) {
        case 2:
            anim = 0xF;
            break;
        case 1:
            anim = 0xE;
            break;
        default:
            anim = (*info)->animIndex;
            break;
        }
    } else {
        anim = (*info)->animIndex;
    }
    if (actor->state == 1) {
        mode = 1;
        randomize = 0;
        switch (actor->type) {
        case 0x17:
            anim = 0x10;
            break;
        case 0x1B:
            anim = 0x12;
            break;
        }
    }
    table = &D_8014D1B4[anim];
    if (mode == 1) {
        if (randomize == mode) {
            actor->frame = func_8002A9B0() % (table->count - 1);
        } else {
            actor->frame = 0;
        }
        actor->frameId = table->frames[actor->frame].id;
        actor->frameNext = table->frames[actor->frame].next;
        actor->timer = table->frames[actor->frame].duration;
        actor->frame++;
    }
}
