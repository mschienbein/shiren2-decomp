#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0xD]; u8 field_D; } Item;
typedef struct { u8 pad_0[0x1E]; u8 field_1E; } Unit;
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800AE674(void *obj);
extern s32 func_80111578(void *obj);
extern void func_800498E4(s32 id, ...);
extern char *func_800A3B20(Unit *unit);
extern char *func_800ACC90(void *obj);
extern void *func_800B31E8(void *pos, s32 team);
s32 func_80111A20(Item *item, Unit *unit)
{
    char *name;
    func_80049CB4(0x1040, unit);
    name = func_800AE674(item);
    if (func_80111578(item) != 0) {
        u8 flags;
        item->field_D++;
        flags = unit->field_1E;
        if ((flags >> 2) & 1) {
            func_800498E4(108, name);
        } else if ((flags >> 4) & 1) {
            char *first = func_800A3B20(unit);
            func_800498E4(109, first, func_800ACC90(item));
        } else {
            func_800498E4(109, func_800A3B20(unit), name);
        }
        if (func_800B31E8(unit, 10) != 0) {
            func_800498E4(231);
            return 0;
        }
        return 1;
    }
    if ((unit->field_1E >> 2) & 1) {
        func_800498E4(110, name);
    } else {
        func_800498E4(111, func_800A3B20(unit), name);
    }
    return 0;
}
