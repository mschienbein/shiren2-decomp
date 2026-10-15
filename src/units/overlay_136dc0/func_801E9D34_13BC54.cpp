#include "func_801E9D34_13BC54/types.h"

/* TU-local classes: unique names, so no other C++ TU defines them.
 * Cleared 8-byte layout value (memset 0x801E9E38, stores 0x801E9E64/0x801E9E68). */
struct Cell801E9D34 : MenuPos {
    Cell801E9D34() {
        func_8006A810(this, 0, sizeof(*this));
    }
};

/* Local menu object: the shared Menu801E9D34 widget view (unchanged in every s07 header)
 * plus its inline base destructor, which restores the base method table D_80151E38
 * (sw 0x801E9F28, after the result load at 0x801E9F1C). The base pointer is passed to
 * func_801E8B10_13AA30 by implicit derived-to-base conversion. */
struct LocalMenu801E9D34 : Menu801E9D34 {
    ~LocalMenu801E9D34() { table_4C = D_80151E38; }
};

extern "C" s32 func_801E9D34_13BC54(void) {
    s32 width = 1;
    s32 i;
    s32 len;
    Object80094C04 *obj;

    if (D_801F51E8_147108->unk_996) {
        obj = func_80094C04(D_80140160, D_801F51E8_147108->unk_99F);
        if (obj != 0) {
            func_800D8FF0(obj);
            obj->table_04->destroy_0C((u8 *)obj + obj->table_04->delta_08, 3);
            return D_801F51E8_147108->unk_995;
        }
    }
    for (i = 0; i < D_801F51E8_147108->unk_99E; i++) {
        len = D_801F51E8_147108->unk_9C8[i];
        len = func_8008387C(&D_801F51E8_147108->text_126[len]);
        if (width < len) {
            width = len;
        }
    }
    MenuSize size;
    const MenuPos &origin = D_80138F80[1];
    size.origin.x = origin.x;
    size.origin.y = origin.y;
    size.rows = D_801F51E8_147108->unk_99E;
    size.width = width + 2;
    MenuPos pos;
    Cell801E9D34 cell;
    cell.x = D_801F51E8_147108->unk_99E;
    cell.y = 1;
    pos = cell;
    LocalMenu801E9D34 menu;
    Menu801E9D34 *self = &menu;
    func_801E8B10_13AA30(self, D_801F5170_147090, &size, &pos, D_801F51E8_147108->unk_99C != 0, D_801F51E8_147108->unk_A00);
    func_80049CB4(0xD);
    func_800957C0(self, D_801F6F20_148E40, 0, 0, D_801F51E8_147108->unk_9FA);
    func_80049CB4(0xE);
    if (D_801F51E8_147108->unk_9F8 == 0) {
        obj = func_80094C04(D_80140160, (s16)menu.result_64);
        if (obj != 0) {
            obj->table_04->destroy_0C((u8 *)obj + obj->table_04->delta_08, 3);
        }
    }
    return (s16)menu.result_64;
}
