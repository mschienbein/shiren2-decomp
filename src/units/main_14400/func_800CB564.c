#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_0; u8 field_1; u8 pad_2[12]; u8 field_E; } Unit;
typedef struct { u8 field_0; u8 field_1; u8 field_2; u8 list[16]; } Record;
extern s32 func_8010B9F4(Unit *unit);
extern s32 func_8010BBF4(Unit *unit, unsigned char *list);
void func_800CB564(Record *record, Unit *unit)
{
    s32 count;
    if (unit == 0) {
        record->field_0 = 0;
    } else {
        record->field_0 = unit->field_1;
        record->field_1 = func_8010B9F4(unit);
        record->field_2 = unit->field_E;
        count = (u8)func_8010BBF4(unit, record->list);
        if (count < 16) {
            record->list[count] = 0;
        }
    }
}
