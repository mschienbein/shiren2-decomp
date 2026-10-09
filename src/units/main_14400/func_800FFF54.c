#include "common.h"
typedef struct Obj800E0F40 Obj800E0F40;
extern s32 func_800E20CC(void *arg0);
extern s32 func_800E0F40(Obj800E0F40 *obj);
extern s32 func_800A529C(void *obj, s32 flag);
/* Actor vtable slot 0xB4 override (D_8015B068+0xB4). Slot callers (e.g. func_800F27A4 at
 * 0x800F2B28) pass the receiver and a target in a1; this override never reads the target. */
s32 func_800FFF54(Obj800E0F40 *obj, void *slot_target) {
    s32 blocked = 0;
    if (!func_800E20CC(obj) || (unsigned char)func_800E0F40(obj) != 3) blocked = 1;
    if (blocked) return 0;
    func_800A529C(obj, 0);
    return 1;
}
