#include "common.h"
typedef struct {
    short field_00, field_02; unsigned char field_04[2]; unsigned char field_06, field_07;
    unsigned char field_08, field_09; unsigned char field_0a[2]; short field_0c, field_0e, field_10;
    unsigned char field_12[0xe]; float field_20;
    unsigned char field_24[0x1a]; unsigned char field_3e, field_3f, field_40;
} Entity;
typedef struct {
    void (*field_00)(void *); short field_04; unsigned short field_06, field_08;
    unsigned char field_0a[8]; unsigned short field_12;
    s32 field_14, field_18; u32 field_1c; s32 field_20, field_24, field_28, field_2c;
    unsigned char field_30[0xc]; float field_3c, field_40, field_44, field_48, field_4c;
    unsigned char field_50[0xc]; s32 field_5c, field_60, field_64, field_68, field_6c, field_70;
} Object;
typedef struct { s32 field_00; unsigned char field_04; } Detail;
extern Object D_801BA380[];
extern Entity *func_8007946C(s32, s32);
extern void func_800892B0(Object *, float);
extern s32 func_80042734(s32), func_800627C4(void), func_800625FC(s32, s32), func_80062554(s32, s32);
extern Detail **func_80074784(s32, s32);
extern s32 func_80084014(s32, s32);
typedef union { s32 word; struct { unsigned short hi, lo; } half; } Pos;
extern void func_80061820(s32, s32), func_80061A20(float), func_8005A234(void);
extern void func_80061908(Pos, Pos);
void func_80088828(Object *self) {
    Entity *entity = func_8007946C(0, self->field_14);
    s32 state = self->field_08;
    switch (state) {
    case 0: {
        s32 found = 0;
        s32 i;
        Object *other;
        for (i = 0; i < self->field_06; i++) {
            other = &D_801BA380[i];
            if (other->field_00 == self->field_00 && other->field_14 == self->field_14) {
                found++;
                break;
            }
        }
        if (!found) {
            self->field_24 = entity->field_09;
            self->field_28 = entity->field_40;
            self->field_2c = entity->field_3e;
            self->field_18 = entity->field_07;
            entity->field_07 = 2;
            if (!(self->field_12 & 0x1000)) {
                entity->field_40 = 1;
                entity->field_3e = 0;
            }
            if (self->field_12 & 0x2000) func_800892B0(self, 1.0f);
            else func_800892B0(self, 2.0f);
            self->field_24 = self->field_1c;
            self->field_08++;
        }
        break;
    }
    case 1:
        if (self->field_1c == 0) {
            s32 x = self->field_60;
            s32 y = self->field_6c;
            s32 kind;
            entity->field_0c = (x << 7) + 0x40;
            entity->field_10 = (y << 7) + 0x40;
            kind = func_80042734(self->field_14);
            switch (kind) {
            case 1:
                if (entity->field_02 == 0x49) {
                    s32 offset = -8;
                    if (func_800627C4() == 2) offset = -4;
                    entity->field_0e = (offset - (s32)((float)(*func_80074784(entity->field_02, entity->field_06))->field_04 * entity->field_20)) * 4;
                } else entity->field_0e = func_80062554(x, y) * 4;
                break;
            case 2:
                if (func_800625FC(x, y) & 0x2000) entity->field_0e = 0;
                else entity->field_0e = func_80062554(x, y) * 4;
                break;
            }
            entity->field_40 = (unsigned char)self->field_28;
            entity->field_3e = (unsigned char)self->field_2c;
            entity->field_09 = (unsigned char)self->field_24;
            entity->field_07 = (unsigned char)self->field_18;
            self->field_04 = 4;
        } else {
            do {
                s32 x, y;
                s32 base_x = (self->field_5c << 7) + 0x40;
                s32 base_y = (self->field_68 << 7) + 0x40;
                self->field_40 -= self->field_3c;
                self->field_4c -= self->field_48;
                entity->field_0c = base_x;
                entity->field_10 = base_y;
                entity->field_0c += (s32)self->field_40;
                entity->field_10 += (s32)self->field_4c;
                x = (entity->field_0c - 0x40) >> 7;
                y = (entity->field_10 - 0x40) >> 7;
                self->field_1c--;
                if (!(self->field_12 & 0x100) || !self->field_1c) break;
                if (func_80084014(x, y)) break;
            } while (1);
        }
        if (self->field_12 & 2) {
            float elapsed = (double)(self->field_24 - self->field_1c);
            float duration = self->field_24;
            func_80061820(self->field_5c, self->field_68);
            { Pos x, y; x.word = self->field_60; y.word = self->field_6c; func_80061908(x, y); }
            if (self->field_5c != self->field_60 || self->field_68 != self->field_6c)
                func_80061A20((elapsed / duration) * 100.0);
            func_8005A234();
        }
        break;
    }
}
