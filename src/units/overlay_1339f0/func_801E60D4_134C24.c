#include "func_801E60D4_134C24/types.h"

extern OvlMenu *func_800953C0(OvlMenu *menu);
extern const u8 D_801E80A0_136BF0[];
extern const u8 D_80152968[];
extern const u8 D_80152AE8[];
extern const u8 D_80151EC8[];
extern const u8 D_801E8130_136C80[];
extern const u8 D_801521D0[];

OvlGroup *func_801E60D4_134C24(OvlGroup *group) {
    OvlMenu *panel0;
    OvlMenu *first0;
    OvlMenu *second0;
    OvlMenu *third0;
    OvlMenu *panel1;
    OvlMenu *first1;
    OvlMenu *second1;
    OvlMenu *third1;
    func_800953C0(&group->menu);
    group->menu.table_4C = D_801E80A0_136BF0;
    panel0 = &group->panels[0].menu;
    func_800953C0(panel0);
    panel0->table_4C = D_80152968;
    first0 = &group->panels[0].first_05C.menu;
    func_800953C0(first0);
    first0->table_4C = D_80152AE8;
    group->panels[0].first_05C.value_68 = -1;
    group->panels[0].first_05C.data_6C = D_80151EC8;
    panel0->table_4C = D_801E8130_136C80;
    second0 = &group->panels[0].second_17C.menu;
    func_800953C0(second0);
    second0->table_4C = D_80152AE8;
    group->panels[0].second_17C.value_68 = -1;
    group->panels[0].second_17C.data_6C = D_80151EC8;
    third0 = &group->panels[0].third_288.menu;
    func_800953C0(third0);
    third0->table_4C = D_801521D0;
    panel1 = &group->panels[1].menu;
    func_800953C0(panel1);
    panel1->table_4C = D_80152968;
    first1 = &group->panels[1].first_05C.menu;
    func_800953C0(first1);
    first1->table_4C = D_80152AE8;
    group->panels[1].first_05C.value_68 = -1;
    group->panels[1].first_05C.data_6C = D_80151EC8;
    panel1->table_4C = D_801E8130_136C80;
    second1 = &group->panels[1].second_17C.menu;
    func_800953C0(second1);
    second1->table_4C = D_80152AE8;
    group->panels[1].second_17C.value_68 = -1;
    group->panels[1].second_17C.data_6C = D_80151EC8;
    third1 = &group->panels[1].third_288.menu;
    func_800953C0(third1);
    third1->table_4C = D_801521D0;
    return group;
}
