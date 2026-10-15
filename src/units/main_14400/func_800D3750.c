#include "common.h"
#include "item_records.h"
typedef struct { void *field_0; unsigned short field_4; } S;
typedef S ListIter800AC6F8;
typedef S Obj800B0864;
extern unsigned short D_8014767C;
extern S *func_800B07F0(S *s);
extern s32 func_800B0808(ListIter800AC6F8 *li);
/* Records of pool D_80143094 (func_800B07F0): flags +2 and signed owner +5. */
extern Item *func_800B0864(Obj800B0864 *obj);
void func_800D3750(void) {
    S iter;
    s32 enabled = (D_8014767C & 0xC) != 0;
    func_800B07F0(&iter);
    while (func_800B0808(&iter)) {
        Item *entry = func_800B0864(&iter);
        if (~entry->owner05) {
            entry->owner05 = -1;
            if (enabled) entry->flags02 |= 0x80;
        }
        if (!enabled) entry->flags02 &= 0x7F;
    }
}
