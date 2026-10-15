#include "func_801E8E00_13AD20/types.h"
extern void *func_800A8CB0(s32 cell);
extern s32 func_800A8974(void *obj);
extern s32 func_80049CB4(s32 id, ...);
void func_801E8E00_13AD20(void) {
    s32 index = 30;
    func_80049CB4(2);
    for (;;) {
        S06Creature *obj;
        if (--index == 0) break;
        obj = func_800A8CB0((u8)index);
        if (func_800A8974(obj)) continue;
        /* ODD_C: retain the ROM's null-preserving +84/+78 subobject
         * conversions, despite the preceding flag read requiring non-null
         * obj. They resemble C++ base conversions, expressed under the
         * required C profile. Direct member access in the same loop measured
         * 188 bytes / 26 differing words, versus 212 original bytes. */
        if ((obj->flags1E >> 3) & 1) {
            S06Subobject *sub = obj ? &obj->sub84 : 0;
            if (sub->field8 == 0) func_80049CB4(0x91, obj);
        }
        if ((obj->flags1E >> 6) & 1) {
            S06Subobject *sub = obj ? &obj->sub78 : 0;
            if (sub->field8 == 0) func_80049CB4(0x91, obj);
        }
    }
    func_80049CB4(2);
}
