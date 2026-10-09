#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad0[0x2C];
    float x, y, scaleX, scaleY;
    s32 field3C, field40, tile;
    u8 pad48[8];
} Sprite;
typedef struct { s32 position; float timer; } ScriptState;
typedef struct { void *walk, *wait, *exit; } Scripts;
typedef struct {
    float y, limit;
    s32 tile;
    Scripts *scripts;
    s32 field10, state, timer;
    float speed, lastEmission;
    s32 turned;
    ScriptState script;
    Sprite sprite;
} FadeTarget;
extern FadeTarget D_801A7228;
extern u32 D_801A7208;
extern s32 D_801A7218, D_801A721C;
extern u8 D_8013D648[], D_8013D650[], D_801E4E48[];
extern s32 func_80042B10(void);
extern u32 func_8006A7A8(u16);
extern s32 func_80070598(void *, void *);
extern s32 func_80073920(void *);
extern float func_80073940(float, float, float, u32, u32 *);
extern s32 func_80073B00(float);
extern s32 func_80073E78(void *, void *, void *, float);
void func_80072F14(void) {
    u32 distance;
    Sprite *sprite = &D_801A7228.sprite;
    u32 progress = (u32)func_80042B10();
    float targetX = 336.0f - ((float)progress * 368.0f) / (float)D_801A7208;
    float speed = func_80073940((float)progress, sprite->x, D_801A7228.limit, D_801A7218, &distance);
    switch (D_801A7228.state) {
    case 0:
        sprite = 0;
        if (func_80073920(&D_801A7228.timer) != 0 && distance >= 240) {
            D_801A7228.state = 1;
            D_801A7228.script.position = 0;
            D_801A7228.script.timer = 0.0f;
        }
        break;
    case 1:
        D_801A7228.speed = speed;
        D_801A7228.state = 2;
        /* fall through */
    case 2: {
        float separation;
        sprite->x += D_801A7228.speed;
        if (sprite->x >= 448.0f) sprite->x = 448.0f;
        sprite->y = D_801A7228.y;
        sprite->tile = D_801A7228.tile;
        func_80073E78(sprite, &D_801A7228.script, D_801A7228.scripts->walk, D_801A7228.speed);
        if (D_801A7228.speed >= 4.0f && sprite->x - D_801A7228.lastEmission >= 8.0f) {
            func_80073B00(sprite->x);
            D_801A7228.lastEmission = sprite->x;
        }
        separation = sprite->x - targetX;
        if (separation < 0.0f) separation = -separation;
        if (D_801A7228.turned == 0) {
            if (sprite->x >= 288.0f) {
                if (func_8006A7A8(3) == 0 && targetX < sprite->x) {
                    D_801A7228.state = 3;
                    if (D_801A721C != 0) D_801A7228.timer = func_8006A7A8(30) + 30;
                    else D_801A7228.timer = func_8006A7A8(30);
                    D_801A7228.script.position = 0;
                    D_801A7228.script.timer = 0.0f;
                } else {
                    D_801A7228.state = 1;
                }
                D_801A7228.turned = 1;
            } else if (speed * 16.0f <= separation) {
                if (targetX < sprite->x) {
                    if (func_8006A7A8(1) == 0) {
                        if (D_801A721C == 1) {
                            D_801A7228.state = 5;
                            D_801A7228.timer = func_8006A7A8(120) + 60;
                            D_801A7228.script.position = 0;
                            D_801A7228.script.timer = 0.0f;
                        } else {
                            D_801A7228.state = 3;
                            if (D_801A721C != 0) D_801A7228.timer = func_8006A7A8(30) + 30;
                            else D_801A7228.timer = func_8006A7A8(30);
                            D_801A7228.script.position = 0;
                            D_801A7228.script.timer = 0.0f;
                        }
                    } else {
                        D_801A7228.state = 3;
                        if (D_801A721C != 0) D_801A7228.timer = func_8006A7A8(30) + 30;
                        else D_801A7228.timer = func_8006A7A8(30);
                        D_801A7228.script.position = 0;
                        D_801A7228.script.timer = 0.0f;
                    }
                } else {
                    D_801A7228.state = 1;
                }
            } else if (!(D_801A7218 & 31) && speed * 8.0f <= separation) {
                s32 action = func_8006A7A8(3);
                if (action == 0) {
                    D_801A7228.state = 1;
                } else if (action == 2) {
                    if (targetX < sprite->x) {
                        D_801A7228.state = 3;
                        if (D_801A721C != 0) D_801A7228.timer = func_8006A7A8(30) + 30;
                        else D_801A7228.timer = func_8006A7A8(30);
                        D_801A7228.script.position = 0;
                        D_801A7228.script.timer = 0.0f;
                    } else {
                        D_801A7228.state = 1;
                    }
                }
            }
        }
        break;
    }
    case 3:
        sprite->tile = D_801A7228.tile;
        func_80073E78(sprite, &D_801A7228.script, D_801A7228.scripts->wait, 1.0f);
        if (func_80073920(&D_801A7228.timer) != 0 && sprite->x <= targetX) {
            D_801A7228.state = 4;
            D_801A7228.script.position = 0;
            D_801A7228.script.timer = 0.0f;
        }
        break;
    case 4:
        sprite->tile = D_801A7228.tile;
        if (func_80073E78(sprite, &D_801A7228.script, D_801A7228.scripts->exit, 1.0f) != 0) {
            D_801A7228.state = 1;
            D_801A7228.script.position = 0;
            D_801A7228.script.timer = 0.0f;
        }
        break;
    case 5:
        func_80073E78(sprite, &D_801A7228.script, D_8013D648, 1.0f);
        if (func_80073920(&D_801A7228.timer) != 0 && sprite->x <= targetX) {
            D_801A7228.state = 6;
            D_801A7228.script.position = 0;
            D_801A7228.script.timer = 0.0f;
        }
        break;
    case 6:
        if (func_80073E78(sprite, &D_801A7228.script, D_8013D650, 1.0f) != 0) {
            D_801A7228.state = 1;
            D_801A7228.script.position = 0;
            D_801A7228.script.timer = 0.0f;
        }
        break;
    }
    if (sprite != 0) func_80070598(D_801E4E48, sprite);
}
