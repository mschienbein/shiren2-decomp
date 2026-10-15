#include "map_rect.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { MapPos field_00; u8 pad_08[4]; MapRect field_0C; u16 field_1C; u8 pad_1E[0x58]; u8 field_76; u8 pad_77[0x29]; MapPos field_A0; } Obj;

extern s32 func_800F0EC4(Obj *obj);
extern s32 func_80102F50(void *obj, void *target);
extern s32 func_800E0F40(Obj *obj);
extern MapPos *func_800B3774(MapPos *out);
extern s32 func_800A31C8(MapRect *rect, MapPos *point);
extern s32 func_800A650C(Obj *obj, MapPos *target);
extern void *func_800A6538(void *out, void *obj, void *target);
extern s32 func_800A4754(Obj *obj, void *arg, u8 *direction);
extern void func_800A665C(Obj *obj, u8 *direction);
extern s32 func_801032C4(Obj *obj);
extern s32 func_800E65C0(Obj *obj, MapPos *target, s32 mode);
s32 func_80103004(Obj *obj)
{
    MapPos temporary;
    u8 direction;
    u8 next_direction;
    MapPos *target;
    s32 allowed;
    s32 active = func_800F0EC4(obj) != 1;
    if (active) {
        MapPos *candidate = &obj->field_A0;
        s32 missing = func_80102F50(obj, candidate) != 1;
        if (missing) {
            s32 use_saved = 0;
            obj->field_A0.x = 0;
            candidate->y = 0;
            if ((u8)func_800E0F40(obj) == 3) {
                use_saved = (D_80142F18.mode & 0xE0) == 0x20;
            }
            if (use_saved) {
                s32 outside;
                func_800B3774(&temporary);
                obj->field_A0 = temporary;
                outside = func_800A31C8(&obj->field_0C, candidate) != 1;
                if (outside) {
                    obj->field_A0.x = 0;
                    candidate->y = 0;
                }
            }
            if ((obj->field_A0.y | obj->field_A0.x) == 0) {
                temporary = func_800A6B70(obj, 2, 0);
                obj->field_A0 = temporary;
            }
        }
        target = &obj->field_A0;
        if ((target->y | obj->field_A0.x) != 0) {
            allowed = 0;
            if (func_800A650C(obj, target) == 1) {
                func_800A6538(&direction, obj, target);
                allowed = func_800A4754(obj, obj, &direction) != 0;
            }
            if (allowed) {
                func_800A6538(&next_direction, obj, target);
                func_800A665C(obj, &next_direction);
                if (func_801032C4(obj)) {
                    obj->field_1C |= 0x200;
                    return 1;
                }
            }
            {
                MapPos *fallback = &obj->field_A0;
                if (func_800E65C0(obj, fallback, 0)) {
                    return 1;
                }
                if (obj->field_76 >= 11) {
                    obj->field_A0.x = 0;
                    fallback->y = 0;
                }
            }
        }
    }
    return 0;
}
