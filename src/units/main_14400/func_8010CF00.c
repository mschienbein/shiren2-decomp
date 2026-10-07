#include "common.h"
typedef struct { unsigned char field_00[0x40]; short field_40; unsigned char (*field_44)(void *, s32); } VTable;
typedef struct { unsigned char field_00[8]; VTable *field_08; } Object;
extern unsigned char D_80156E3C[];
extern s32 func_8010BEC4(Object *, unsigned char);
s32 func_8010CF00(Object *object, s32 kind, s32 value) {
    switch (kind) {
    case 0: { s32 result = object->field_08->field_44((char *)object + object->field_08->field_40, 0x5A); if (!result) result = value; return result; }
    case 1: if ((unsigned char)func_8010BEC4(object, 0x66)) return 100; break;
    case 2: {
        s32 a, difference;
        if ((unsigned char)func_8010BEC4(object, 0x6E)) return 0;
        a = (unsigned char)func_8010BEC4(object, 0x5F);
        difference = (unsigned char)func_8010BEC4(object, 0x5B) - a;
        if (difference > 0) { value -= value * D_80156E3C[difference - 1] / 100; if (value < 0) value = 1; }
        else if (difference < 0) value *= 2;
        break;
    }
    }
    return value;
}
