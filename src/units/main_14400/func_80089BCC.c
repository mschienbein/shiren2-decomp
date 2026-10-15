#include "common.h"

/* Sprite record from func_8007946C (field view shared with func_80088828). */
typedef struct {
    short field_00, field_02;
    unsigned char field_04[2];
    unsigned char field_06, field_07, field_08, field_09;
    unsigned char field_0a[2];
    short field_0c, field_0e, field_10;
    unsigned char field_12[2];
    float field_14;
    unsigned char field_18[4];
    float field_1c;
    float field_20;
    unsigned char field_24[0x18];
    unsigned short field_3c;
    unsigned char field_3e;
} Entity;

/* Effect task (field view shared with func_80088828 / func_800893D8). */
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

typedef union { s32 word; struct { unsigned short hi, lo; } half; } Pos;

typedef struct CamParams CamParams;

extern Entity *func_8007946C(s32, s32);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern s32 func_8005B07C(void);
extern void func_8005A464(s32, CamParams *, s32, s32);
extern s32 func_800751B4(s32, s32);
extern s32 func_80062554(s32, s32);
extern float func_80032360(float);
extern void func_80061820(s32, s32), func_80061A20(float), func_8005A234(void);
extern void func_80061908(Pos, Pos);

void func_80089BCC(Object *self) {
    Entity *entity = func_8007946C(0, self->field_14);
    float angle = 50.0f; /* launch angle in degrees */
    s32 x;
    s32 y;
    u32 remaining;

    switch (self->field_08) {
    case 0: {
        float dx, dy, steps, velocity, height, gravity;
        s32 count;
        u32 i;

        x = self->field_5c;
        y = self->field_68;

        if (self->field_12 & 0x3000) {
            func_80076044(self->field_14, entity->field_3c, 2, entity->field_3e, 3);
        } else {
            func_80076044(self->field_14, entity->field_3c, 2, entity->field_3e, 0);
        }
        if (self->field_12 & 2) {
            self->field_18 = func_8005B07C();
            func_8005A464(self->field_14, 0, 0, 0);
        }
        entity->field_0c = (x << 7) + 0x40;
        entity->field_10 = (y << 7) + 0x40;
        if (self->field_12 & 0x3000) {
            func_800751B4(self->field_14, 0x8000);
            entity->field_0e = 0;
        } else {
            entity->field_0e = func_80062554(x, y) * 4;
        }
        dx = (float)(self->field_5c - self->field_60) * 128.0f;
        self->field_30 = 0.01f;
        self->field_34 = 0.75f;
        dy = (float)(self->field_68 - self->field_6c) * 128.0f;
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
        self->field_34 = entity->field_1c;
        if (self->field_12 & 0x2000) {
            for (i = 0; i < self->field_1c; i++) {
                entity->field_1c = entity->field_20 = entity->field_1c * 0.8333333f;
            }
        }
        self->field_3c = dx / steps;
        self->field_48 = dy / steps;
        self->field_40 = 0.0f;
        self->field_4c = 0.0f;
        self->field_24 = self->field_1c;
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
        entity->field_0c += (s32)self->field_40;
        entity->field_10 += (s32)self->field_4c;
        entity->field_14 = (s32)(self->field_58 * 1000.0f * 3.0f);
        if (self->field_12 & 0x1000) {
            entity->field_1c = entity->field_20 = entity->field_1c * 0.8f;
        } else if (self->field_12 & 0x2000) {
            entity->field_1c = entity->field_20 = entity->field_1c * 1.2f;
        }
        if (self->field_12 & 2) {
            float elapsed = (double)(self->field_24 - self->field_1c);
            float duration = self->field_24;
            func_80061820(self->field_5c, self->field_68);
            { Pos px, py; px.word = self->field_60; py.word = self->field_6c; func_80061908(px, py); }
            if (self->field_5c != self->field_60 || self->field_68 != self->field_6c) {
                func_80061A20((elapsed / duration) * 100.0);
            }
            func_8005A234();
        }
        remaining = self->field_1c--;
        if (remaining == 0) {
            s32 tx = self->field_60;
            s32 ty = self->field_6c;
            if (self->field_12 & 2) {
                func_8005A464(self->field_18, 0, 0, 0);
                entity->field_0c = (tx << 7) + 0x40;
                entity->field_10 = (ty << 7) + 0x40;
                entity->field_0e = func_80062554(tx, ty) * 4;
                if (self->field_12 & 0x3000) {
                    func_800751B4(self->field_14, 0);
                }
                func_80061820(tx, ty);
                { Pos px, py; px.word = tx; py.word = ty; func_80061908(px, py); }
            }
            self->field_0e = 1;
            self->field_04 = 4;
            self->field_08++;
        }
        break;
    }
}
