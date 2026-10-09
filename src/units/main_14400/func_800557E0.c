#include "common.h"
typedef struct {
    unsigned char pad_00[0x44]; s32 id;
    unsigned char pad_48[0x14]; short active; short field_5E;
} Task;
extern Task D_801D40DC[32];
s32 D_80139B18 = 0;
extern void func_80055BD8(s32);
extern void func_80055900(void);
void func_800557E0(void) {
    s32 i;
    for (i = 0; i < 32; i++) {
        if (D_801D40DC[i].id != -1 && D_801D40DC[i].active == 1) func_80055BD8(i);
    }
    D_80139B18 = 1;
    func_80055900();
}
