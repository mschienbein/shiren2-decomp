#include "func_801E62AC_134DFC/types.h"

extern OvlMenu *func_800953C0(OvlMenu *menu);
extern const u8 D_80152968[];
extern const u8 D_80152AE8[];
extern const u8 D_80151EC8[];
extern const u8 D_801E8130_136C80[];
extern const u8 D_801521D0[];

OvlOwner *func_801E62AC_134DFC(OvlOwner *owner) {
    OvlMenu *first;
    OvlMenu *second;
    OvlMenu *third;
    func_800953C0(&owner->menu);
    owner->menu.table_4C = D_80152968;
    first = &owner->first_05C.menu;
    func_800953C0(first);
    first->table_4C = D_80152AE8;
    owner->first_05C.value_68 = -1;
    owner->first_05C.data_6C = D_80151EC8;
    owner->menu.table_4C = D_801E8130_136C80;
    second = &owner->second_17C.menu;
    func_800953C0(second);
    second->table_4C = D_80152AE8;
    owner->second_17C.value_68 = -1;
    owner->second_17C.data_6C = D_80151EC8;
    third = &owner->third_288.menu;
    func_800953C0(third);
    third->table_4C = D_801521D0;
    return owner;
}
