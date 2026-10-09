#include "common.h"
typedef struct { void *field_0; unsigned short field_4; } S;
typedef S ListIter800AC6F8;
typedef S Obj800B0864;
typedef struct Entry800B0864 { unsigned char pad0[2]; unsigned char field_2; unsigned char pad3[2]; signed char field_5; } Entry800B0864;
extern unsigned short D_8014767C;
extern S *func_800B07F0(S *s);
extern s32 func_800B0808(ListIter800AC6F8 *li);
extern Entry800B0864 *func_800B0864(Obj800B0864 *obj);
void func_800D3750(void) {
    S iter;
    s32 enabled = (D_8014767C & 0xC) != 0;
    func_800B07F0(&iter);
    while (func_800B0808(&iter)) {
        Entry800B0864 *entry = func_800B0864(&iter);
        if (~entry->field_5) {
            entry->field_5 = -1;
            if (enabled) entry->field_2 |= 0x80;
        }
        if (!enabled) entry->field_2 &= 0x7F;
    }
}
