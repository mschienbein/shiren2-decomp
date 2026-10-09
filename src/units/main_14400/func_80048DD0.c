#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Vec2;
typedef struct { u8 field_0; u8 field_1; } Mode;
extern Mode D_801390C0[];
extern s32 D_8013968C;

extern Vec2 *func_800C5F60(void);
extern s32 func_800A23E8(Vec2 *origin, Vec2 *vec);
extern s32 func_800A99D0(void);
static __inline__ s32 alternate_mode(void)
{
    return (D_80142F18.flags >> 2) & 1;
}
s32 func_80048DD0(Vec2 *first, Vec2 *second)
{
    Vec2 origin;
    Vec2 target;
    s32 mode = D_801390C0[D_8013968C].field_1;
    s32 result = 1;
    if (mode != 0) {
        s32 distant = 0;
        Vec2 *source = func_800C5F60();
        Vec2 *point = &origin;
        point->x = source->x;
        point->y = source->y;
        target.x = first->x;
        target.y = first->y;
        if (func_800A23E8(point, &target) >= 6) {
            target.x = second->x;
            target.y = second->y;
            distant = (func_800A23E8(point, &target) < 6) ^ 1;
        }
        if (distant != 0) {
            s32 enabled;
            result = 0;
            enabled = 0;
            if (func_800A99D0() != 0) {
                enabled = alternate_mode() == 0;
            }
            if (enabled != 0) {
                result = 1;
            }
        }
    }
    return result;
}
