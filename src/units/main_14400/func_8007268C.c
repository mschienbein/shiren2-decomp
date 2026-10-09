#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef union { u32 word; u8 channel[4]; } Color;
typedef struct {
    u8 type, visible, sort, pad3;
    s32 mode;
    u32 flags, render1, render2;
    Color primitive, environment;
    u8 combine[16];
    float x, y, scaleX, scaleY;
    u8 field3C, pad3D[3];
    s32 field40, tile, field48;
    u8 bank, group, variant, slot;
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
typedef struct {
    s32 state;
    float y, speed;
    s32 timer;
    ScriptState script;
    u32 phase, field1C;
    Sprite sprite;
} Follower;
extern FadeTarget D_801A7228;
extern Follower D_801A72A8;
typedef struct Thread Thread;
typedef struct { Thread *receive_waiters, *send_waiters; long count, first, capacity; void **messages; } MesgQueue;
extern u32 D_801A71C4, D_801A7208;
extern s32 D_801A720C, D_801A7210, D_801A7218, D_801A721C, D_801A7220;
extern u8 *D_801A71C8;
extern Thread *D_801A71E8;
extern MesgQueue D_801A71CC, D_801A71EC;
extern void *D_801A71E4, *D_801A7204;
extern u8 D_8013D520[];
extern Scripts D_8013D658, D_8013D6B8, D_8013D728, D_8013D798, D_8013D808, D_8013D878;
extern s32 func_80042B10(void), func_80042B3C(void);
extern u8 func_80041F68(s32), func_80041EF0(s32);
extern void func_8006A750(s32);
extern u32 func_8006A75C(void), func_8006A7A8(u16);
extern void func_800265E0(void *, s32);
extern void func_800557E0(void), func_800559F8(s32), func_8005C890(s32, s32, s32);
extern void func_80060C54(u32), func_8006E740(s32);
extern void func_80027EA0(MesgQueue *, void **, long);
extern void func_80027ED0(Thread *, s32, void (*)(void *), void *, void *, s32);
extern void func_80032B50(Thread *);
/* This OS thread entry receives an unused void* argument, as this callsite proves. */
extern void func_80072D64(void *);
/* Render-mode word pair (cycle 1 / cycle 2) of a sprite. */
static inline void set_render_mode(Sprite *sprite, u32 render1, u32 render2) {
    sprite->render2 = render2;
    sprite->render1 = render1;
}
/* Common opaque-sprite setup shared by the fade target and its follower. */
static inline void init_sprite(Sprite *sprite) {
    sprite->field40 = 0;
    sprite->field3C = 0;
    sprite->type = 2;
    sprite->visible = 1;
    sprite->flags = 0x200004;
    sprite->mode = 0;
    set_render_mode(sprite, 0x443048, 0x113048);
    sprite->combine[0] = 0x1F;
    sprite->combine[1] = 0x1F;
    sprite->combine[2] = 0x1F;
    sprite->combine[3] = 1;
    sprite->combine[4] = 7;
    sprite->combine[5] = 7;
    sprite->combine[6] = 7;
    sprite->combine[7] = 1;
    sprite->combine[8] = 0x1F;
    sprite->combine[9] = 0x1F;
    sprite->combine[10] = 0x1F;
    sprite->combine[11] = 1;
    sprite->combine[12] = 7;
    sprite->combine[13] = 7;
    sprite->combine[14] = 7;
    sprite->combine[15] = 1;
    sprite->bank = 0;
    sprite->group = 0xFE;
    sprite->variant = 0;
}
static inline void set_position(Sprite *sprite, float limit, float y, float scale) {
    D_801A7228.limit = limit;
    D_801A7228.y = y;
    sprite->y = y;
    sprite->scaleX = scale;
    sprite->scaleY = scale;
}
void func_8007268C(void) {
    s32 total;
    u8 kind;

    if (D_801A71C4 == 0) {
        func_8006A750(func_80042B3C());
        func_8006A75C();
        func_8006A75C();
        func_8006A75C();
        total = func_80042B10();
        D_801A7208 = total;
        D_801A720C = 0;
        D_801A7210 = 0x10;
        D_801A7218 = 0;
        D_801A721C = 0;
        if (total != 0) {
            kind = D_8013D520[func_8006A7A8(9)];
            D_801A721C = (s32) kind;
            switch (kind) {
            case 2:
                if (func_80041F68(2) < 100) D_801A721C = 1;
                break;
            case 3:
                if (!func_80041EF0(0x25)) D_801A721C = 1;
                break;
            case 4:
                if (func_80041F68(2) < 160) D_801A721C = 1;
                break;
            case 5:
                if (!func_80041EF0(0x23)) D_801A721C = 1;
                break;
            case 6:
                if (!func_80041EF0(0x24)) D_801A721C = 1;
                break;
            }
            D_801A7220 = 0;
            if (func_8006A7A8(0x14) == 8) {
                D_801A7220 = 2;
            }
        }
        if (D_801A721C != 0) {
            Sprite *sprite = &D_801A7228.sprite;
            func_800265E0(&D_801A7228, sizeof(D_801A7228));
            D_801A7228.timer = 0x20;
            D_801A7228.state = 0;
            init_sprite(sprite);
            sprite->x = -32.0f;
            switch (D_801A721C) {
            case 1:
                set_position(sprite, 8.0f, 186.0f, 0.5f);
                D_801A7228.tile = 0x8D;
                D_801A7228.scripts = &D_8013D658;
                break;
            case 2:
                set_position(sprite, 6.0f, 182.0f, 0.45f);
                D_801A7228.tile = 0x8F;
                D_801A7228.scripts = &D_8013D6B8;
                break;
            case 3:
                set_position(sprite, 4.0f, 182.0f, 0.5f);
                D_801A7228.tile = 0x90;
                D_801A7228.scripts = &D_8013D728;
                break;
            case 4:
                set_position(sprite, 6.0f, 182.0f, 0.45f);
                D_801A7228.tile = 0xA4;
                D_801A7228.scripts = &D_8013D798;
                break;
            case 5:
                set_position(sprite, 6.0f, 183.0f, 0.5f);
                D_801A7228.tile = 0x7C;
                D_801A7228.scripts = &D_8013D808;
                sprite->bank = 3;
                break;
            case 6:
                set_position(sprite, 2.0f, 170.6f, 0.6f);
                D_801A7228.tile = 0x91;
                D_801A7228.scripts = &D_8013D878;
                break;
            }
            if (D_801A7220 != 0) {
                Sprite *body = &D_801A72A8.sprite;
                Follower *follower = &D_801A72A8;
                func_800265E0(follower, sizeof(*follower));
                init_sprite(body);
                switch (D_801A7220) {
                case 1:
                    follower->state = 0;
                    follower->timer = 0x20;
                    D_801A72A8.y = 182.9f;
                    body->x = -80.0f;
                    body->scaleX = 0.45f;
                    body->scaleY = 0.45f;
                    break;
                case 2:
                    follower->state = 0;
                    set_render_mode(body, 0x4041C8, 0x1041C8);
                    body->combine[0] = 1;
                    body->combine[1] = 0x1F;
                    body->combine[2] = 3;
                    body->combine[3] = 0x1F;
                    body->combine[4] = 1;
                    body->combine[5] = 7;
                    body->combine[6] = 3;
                    body->combine[7] = 7;
                    body->combine[8] = 1;
                    body->combine[9] = 0x1F;
                    body->combine[10] = 3;
                    body->combine[11] = 0x1F;
                    body->combine[12] = 1;
                    body->combine[13] = 7;
                    body->combine[14] = 3;
                    body->combine[15] = 7;
                    body->primitive.word = -1;
                    follower->y = 168.5f;
                    body->scaleX = 0.45f;
                    body->scaleY = 0.45f;
                    body->x = D_801A7228.sprite.x - 80.0f;
                    break;
                }
            }
        }
        func_800557E0();
        func_800559F8(1);
        func_8005C890(0, 0, 8);
        func_80060C54(9);
        func_8006E740(0);
        func_80027EA0(&D_801A71EC, &D_801A7204, 1);
        D_801A71C4 = 1;
        func_80027EA0(&D_801A71CC, &D_801A71E4, 1);
        func_80027ED0(D_801A71E8, 2, &func_80072D64, 0, D_801A71C8 + 0x2000, 0x37);
        func_80032B50(D_801A71E8);
    }
}
