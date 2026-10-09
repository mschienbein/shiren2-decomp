#include "common.h"

typedef struct { s32 field00, field04; } Position;
typedef struct Dir { unsigned char value; } Dir;
typedef struct { s32 unknown00[2]; unsigned short field08; unsigned char unknown0a[14]; } Iterator;
typedef struct { unsigned char unknown00[0x18]; short adjust18; short unknown1a; s32 (*method1c)(void *, s32); } VTable;
typedef struct { s32 unknown00[2]; VTable *field08; } Object;
extern void *func_800C5280(Iterator *, Position *, unsigned short);
extern void *func_800C532C(Position *, Iterator *);
extern s32 func_800C559C(Iterator *), func_800AD714(Object *, Position *), func_800B56F0(Position *);
extern u32 func_800B1C6C(Position *);
extern void *func_800A27A4(unsigned char *, Position *, Position *);
extern void *func_800A2594(Position *, Position *, Dir);
s32 func_800ADD50(Object *object, Position *result) {
    Position origin, candidate, fallback;
    Iterator iterator;
    Position current;
    Dir direction;
    s32 avoid;
    VTable *table;
    Position *start = &origin;
    origin.field00 = result->field00;
    start->field04 = result->field04;
    table = object->field08;
    avoid = table->method1c((char *)object + table->adjust18, 0x24);
    fallback.field00 = 0;
    fallback.field04 = 0;
    current.field00 = origin.field00;
    current.field04 = start->field04;
    func_800C5280(&iterator, &current, 2);
    while (func_800C559C(&iterator)) {
        s32 distance = iterator.field08;
        func_800C532C(&current, &iterator);
        candidate = current;
        if (!func_800AD714(object, &candidate)) continue;
        if (distance == 2) {
            current.field00 = candidate.field00;
            current.field04 = candidate.field04;
            func_800A27A4(&direction.value, &origin, &current);
            func_800A2594(&current, &origin, direction);
            if (func_800B1C6C(&current) & 0x4000) continue;
        }
        if (avoid && func_800B56F0(&candidate)) {
            if (!(fallback.field04 | fallback.field00)) fallback = candidate;
        } else {
            *result = candidate;
            return 1;
        }
    }
    {
        s32 found;
        if (!(fallback.field04 | fallback.field00)) {
            found = 0;
        } else {
            *result = fallback;
            found = 1;
        }
        return found;
    }
}
