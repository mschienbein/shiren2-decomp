#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Point;
/* Complete 0x50-byte sprite record; only the fields written here are named. */
typedef struct {
    u8 pad_00[0x14];
    u32 color;
    u8 pad_18[4], combine[16];
    float x, y, scale_x, scale_y;
    u8 pad_3C[4];
    s32 layer, image, palette;
    u8 enabled, alpha, field_4E, field_4F;
} Sprite;

#define PI 3.141592654

extern s32 D_8013A298;
extern const u8 *D_8013A744[9];
extern Point D_8013A768[9], D_8013A7B0[];
extern u8 D_801630D8[0x28];
extern Sprite *D_80163100;
extern u32 D_80163104;
void func_800584EC(Sprite *sprite);
float func_80032360(float x); /* sinf */
s32 func_80070598(void *object, void *value);
s32 func_80042AD8(s32 id);
s32 func_80042AF4(s32 index);

/* Draws the bobbing menu cursor and one marker per not-yet-available entry
 * of the current list. */
void *func_800581E8(void *cursor) {
    Sprite *sprite = D_80163100;
    const u8 *list = D_8013A744[D_8013A298];
    float baseline, wave;
    s32 next, active, id;

    func_800584EC(sprite);
    sprite->x = D_8013A768[D_8013A298].x;
    baseline = D_8013A768[D_8013A298].y;
    wave = func_80032360((float)(((double)(float)D_80163104 * PI) / 12.0));
    sprite->layer = 100;
    sprite->image = 0x18C;
    sprite->palette = 0;
    sprite->enabled = 0;
    sprite->alpha = 0xFE;
    sprite->y = (s32)(baseline - 2.0f * wave);
    func_80070598(D_801630D8, sprite);
    next = 1;
    for (id = *list++; id != 0xFF; id = *list++) {
        if (id >= 0x3A) {
            active = func_80042AF4(id - 0x3A);
        } else {
            active = 0;
            if (id + 0x1D == 0x52) {
                if (func_80042AD8(0x52)) active = 1;
                else if (func_80042AD8(0x53)) active = 1;
            } else {
                active = func_80042AD8(id + 0x1D);
            }
        }
        if (!active) {
            sprite = &D_80163100[next++];
            func_800584EC(sprite);
            sprite->x = D_8013A7B0[id].x;
            sprite->y = D_8013A7B0[id].y;
            sprite->image = 0x18A;
            sprite->palette = id;
            sprite->enabled = 0;
            sprite->alpha = 0xFE;
            sprite->combine[0] = 1;
            sprite->combine[1] = 31;
            sprite->combine[2] = 3;
            sprite->combine[3] = 31;
            sprite->combine[4] = 7;
            sprite->combine[5] = 7;
            sprite->combine[6] = 7;
            sprite->combine[7] = 1;
            sprite->combine[8] = 1;
            sprite->combine[9] = 31;
            sprite->combine[10] = 3;
            sprite->combine[11] = 31;
            sprite->combine[12] = 7;
            sprite->combine[13] = 7;
            sprite->combine[14] = 7;
            sprite->combine[15] = 1;
            sprite->color = 0x26282A00;
            func_80070598(D_801630D8, sprite);
        }
    }
    D_80163104 = (D_80163104 + 1) % 12;
    return cursor;
}
