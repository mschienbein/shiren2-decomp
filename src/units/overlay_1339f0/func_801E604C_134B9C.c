#include "func_801E604C_134B9C/types.h"

extern const u8 D_801E80A0_136BF0[];
extern const u8 D_801E8130_136C80[];
extern const u8 D_80151E38[144];
extern const u8 D_80152968[];
extern void func_800D8FA8(void *object);

/* ODD_C: explicit destructor table transitions for the two panel bases.
 * Separate base-subobject lifetimes retain the original addressing and stores:
 * direct members gave 29 differing words/128 bytes, one inline panel destructor
 * 21/136, and a reused base pointer 14/136. This form gives 0/136.
 * The intermediate derived/base table stores are unobservable without a call,
 * but mirror the corresponding complete panel destructor (func_801E6258_134DA8).
 * ROM stores: 801E606C/801E60BC (group), 801E6080/608C/6094 (panel 1),
 * 801E60A0/60AC/60B4 (panel 0), members 801E6084/6088/6090, 801E60A4/60A8/60B0. */
void func_801E604C_134B9C(OvlGroup *group, s32 flags) {
    OvlMenu *second;
    OvlMenu *first;
    group->menu.table_4C = D_801E80A0_136BF0;
    second = &group->panels[1].menu;
    second->table_4C = D_801E8130_136C80;
    group->panels[1].third_288.menu.table_4C = D_80151E38;
    group->panels[1].second_17C.menu.table_4C = D_80151E38;
    second->table_4C = D_80152968;
    group->panels[1].first_05C.menu.table_4C = D_80151E38;
    second->table_4C = D_80151E38;
    first = &group->panels[0].menu;
    first->table_4C = D_801E8130_136C80;
    group->panels[0].third_288.menu.table_4C = D_80151E38;
    group->panels[0].second_17C.menu.table_4C = D_80151E38;
    first->table_4C = D_80152968;
    group->panels[0].first_05C.menu.table_4C = D_80151E38;
    first->table_4C = D_80151E38;
    group->menu.table_4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(group);
    }
}
