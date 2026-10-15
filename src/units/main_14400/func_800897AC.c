#include "common.h"

/* Sprite record from func_8007946C (field view shared with func_80089BCC). */
typedef struct {
    short field_00, field_02;
    unsigned char field_04[2];
    unsigned char field_06, field_07, field_08, field_09;
    unsigned char field_0a[2];
    short field_0c, field_0e, field_10;
    unsigned char field_12[2];
    float field_14;
} Entity;

/* Effect task (field view shared with func_80089BCC). */
typedef struct {
    void (*field_00)(void *);
    short field_04;
    unsigned short field_06, field_08;
    unsigned char field_0a[4];
    short field_0e;
    unsigned char field_10[2];
    unsigned short field_12;
    s32 field_14, field_18;
    u32 field_1c;
    s32 field_20, field_24, field_28, field_2c;
    float field_30, field_34, field_38, field_3c, field_40, field_44, field_48, field_4c;
    float field_50, field_54, field_58;
    s32 field_5c, field_60, field_64, field_68, field_6c, field_70;
} Object;

extern Entity *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
extern s32 func_80062554(s32, s32);
extern float func_80032360(float);
extern void func_8007935C(s32);

void func_800897AC(Object *self) {
    Entity *entity = func_8007946C(4, self->field_14);
    s32 x;
    s32 y;
    u32 remaining;

    switch (self->field_08) {
    case 0: {
        float angle, dx, dy, steps, velocity, height, gravity;
        s32 count;

        angle = 65.0f; /* launch angle in degrees */
        if (self->field_12 & 0x1000) {
            angle = 35.0f;
        }
        x = self->field_5c;
        y = self->field_68;
        func_80079560(4, self->field_14, 0);
        entity->field_0c = (x << 7) + 0x40;
        entity->field_10 = (y << 7) + 0x40;
        entity->field_0e = (self->field_12 & 0x2000) ? -0x80 : func_80062554(x, y) * 4;
        dx = (float)(self->field_5c - self->field_60) * 128.0f;
        self->field_30 = 0.01f;
        self->field_34 = 0.48f;
        dy = (float)(self->field_68 - self->field_6c) * 128.0f;
        /* The original executes both stores to field_54 (+0x54): 0x8008998C
         * (swc1 $f6, the launch velocity) and 0x80089990 (swc1 $f2, the
         * gravity-adjusted value). The first is overwritten with no load in
         * between; both assignments are kept as in the original. */
        self->field_54 = self->field_34 * func_80032360(angle * 3.1415 / 180.0);
        self->field_54 -= self->field_30 * 0.5f * 9.8f;
        self->field_58 = self->field_54 * self->field_30;
        velocity = self->field_54;
        height = self->field_58;
        gravity = -9.8f;
        count = 0;
        do {
            velocity += self->field_30 * gravity;
            height += velocity * self->field_30;
            count++;
        } while (height * 1000.0f >= 0.0f);
        count--;
        steps = count;
        count--;
        self->field_1c = count;
        if (steps == 0.0f) {
            self->field_3c = 0.0f;
            self->field_48 = 0.0f;
        } else {
            self->field_3c = dx / steps;
            self->field_48 = dy / steps;
        }
        self->field_40 = 0.0f;
        self->field_4c = 0.0f;
        self->field_08++;
    }
        /* fall through */
    case 1:
        self->field_54 -= self->field_30 * 9.8f;
        self->field_58 += self->field_54 * self->field_30;
        self->field_40 -= self->field_3c;
        self->field_4c -= self->field_48;
        x = self->field_5c;
        y = self->field_68;
        entity->field_0c = (x << 7) + 0x40;
        entity->field_10 = (y << 7) + 0x40;
        entity->field_0e = (self->field_12 & 0x2000) ? -0x80 : func_80062554(x, y) * 4;
        entity->field_0c += (s32)self->field_40;
        entity->field_10 += (s32)self->field_4c;
        if (self->field_12 & 0x2000) {
            entity->field_14 = (s32)(self->field_58 * 1000.0f * 2.0f);
        } else {
            entity->field_14 = (s32)(self->field_58 * 1000.0f * 3.5f);
        }
        remaining = self->field_1c--;
        if (remaining == 0) {
            self->field_08++;
        }
        break;
    case 2:
        func_8007935C(self->field_14);
        self->field_04 = 4;
        break;
    }
}
