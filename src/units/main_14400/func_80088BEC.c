#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned short field_0C; unsigned short field_0E; unsigned short field_10; } Position;
typedef struct { s32 field_00; unsigned short field_04; unsigned short field_06; unsigned short field_08; unsigned char pad_0A[0xA]; s32 field_14; s32 field_18; s32 field_1C; s32 field_20; s32 field_24; unsigned char pad_28[0x14]; float field_3C; float field_40; s32 field_44; float field_48; float field_4C; unsigned char pad_50[0xC]; s32 field_5C[3]; s32 field_68[3]; } Object;
extern Position *func_8007946C(s32 kind, s32 index);
extern float __builtin_sqrtf(float value);
static inline void set_position(Position *position, s32 x, s32 y) {
    position->field_0C = x;
    position->field_10 = y;
}
void func_80088BEC(Object *object) {
    Position *position = func_8007946C(0, object->field_14);
    s32 index;
    switch (object->field_08) {
        case 0:
            object->field_24 = 0;
            object->field_08++;
            break;
        case 1: {
            s32 dx, dy;
            index = object->field_24;
            dx = object->field_5C[index] - object->field_5C[index + 1];
            dy = object->field_68[index] - object->field_68[index + 1];
            if (dx == 0 && dy == 0) object->field_04 = 4;
            else {
                float x = dx << 7;
                float y = dy << 7;
                float distance = __builtin_sqrtf(x * x + y * y) * 0.015625f;
                object->field_40 = 0.0f;
                object->field_4C = 0.0f;
                object->field_08++;
                object->field_1C = (s32)distance - 1;
                object->field_3C = x / distance;
                object->field_48 = y / distance;
            }
            break;
        }
        case 2: {
            index = object->field_24;
            if (object->field_1C-- != 0) {
                object->field_40 -= object->field_3C;
                object->field_4C -= object->field_48;
                set_position(position, (object->field_5C[index] << 7) + 0x40,
                             (object->field_68[index] << 7) + 0x40);
                position->field_0C += (s32)object->field_40;
                position->field_10 += (s32)object->field_4C;
            } else if (index == 0) {
                if (object->field_5C[index + 1] != object->field_5C[index + 2] ||
                    object->field_68[index + 1] != object->field_68[index + 2]) {
                    object->field_24++;
                    object->field_08--;
                } else object->field_04 = 4;
            } else object->field_04 = 4;
            break;
        }
    }
}
