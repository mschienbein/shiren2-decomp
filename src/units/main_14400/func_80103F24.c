#include "common.h"
typedef struct Object Object;
struct Object { unsigned char field_00[9]; unsigned char field_09; unsigned char field_0A[0x12]; unsigned short field_1C; unsigned char field_1E[0x7C]; unsigned short field_9A; s32 field_9C; Object *field_A0; };
typedef struct { s32 words[4]; } Region;
typedef struct { s32 words[8]; } Iterator;
extern void *func_800A2FD0(Region *region, Object *obj, unsigned char mode);
extern Iterator *func_800A9204(Iterator *iterator, Region *region, Object *obj);
extern s32 func_800A9284(Iterator *iterator, s32 filter);
extern Object *func_800A942C(Iterator *iterator);
extern s32 func_800A4520(Object *obj, Object *other);
extern s32 func_800A455C(Object *obj, Object *other, s32 mode);
extern s32 func_800E1CC4(Object *obj, s32 mode);
extern u32 func_800B1C6C(Object *obj);
s32 func_80103F24(Object *obj)
{
    Region region;
    Iterator iterator;
    Object *other;
    s32 valid;
    func_800A2FD0(&region, obj, 1);
    func_800A9204(&iterator, &region, obj);
    while (func_800A9284(&iterator, 0x7C)) {
        other = func_800A942C(&iterator);
        valid = 0;
        if (!(obj->field_9A & 0x40) || func_800A4520(obj, other)) {
            if (func_800A455C(obj, other, 1) && !func_800E1CC4(other, 1) && !(other->field_1C & 1)) {
                if (!(func_800B1C6C(other) & 0x80) || (other->field_09 & 15) == 2) valid = 1;
            }
        }
        if (valid) {
            obj->field_A0 = other;
            return 1;
        }
    }
    return 0;
}
