#include "row_views.h"

extern s32 D_8013E814;
extern s32 D_8013E818;
extern s32 D_8013E8DC;
extern u16 D_801A9058[];
void func_80082150(void);
void func_800821D8(u32 id) {
    s32 i;
    u16 state;
    u16 cur;
    func_80082150();
    if (id < 10 && D_801A9080[id].field_02 != 0) {
        state = 0x41;
        for (i = D_8013E814; i < D_8013E818; i++) {
            cur = D_801A9058[i];
            if (cur == id) {
                state = 0x40;
            }
            D_801A9080[cur].field_02 = state;
        }
        if (D_8013E818 > D_8013E814) {
            D_8013E8DC = 1;
        }
    }
}
