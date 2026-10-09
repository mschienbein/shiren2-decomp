#include "common.h"

typedef struct { unsigned char pad00[0xA]; unsigned char kind0A; } Obj800E0F40;
typedef struct { unsigned char pad00[0x1E]; unsigned char flags1E; } Target;
typedef struct { unsigned char pad00[0xA]; unsigned short value0A; } Record;
extern s32 func_800E0F40(Obj800E0F40 *obj);
extern Record *func_800451A8(unsigned char kind, unsigned char value);

s32 func_800F4208(Obj800E0F40 *obj, Target *target)
{
    s32 value;
    Record *record;
    s32 kind;
    if (target == 0) {
        return 0;
    }
    kind = obj->kind0A;
    record = func_800451A8(kind, func_800E0F40(obj));
    value = 1;
    if (target->flags1E & 0xC) {
        value = record->value0A;
    }
    return value;
}
