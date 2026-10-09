#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    void *field_0;
    u16 index_4;
    u16 pad6;
} Iter801169D0;

typedef struct {
    u8 kind_0;
} Item801169D0;

u8 D_801486E4 = 0;
extern u8 D_801486DC;
extern s32 D_801486E0;
extern void *D_801476B8;
s32 func_80049CB4(s32 id, ...);
void *func_800B07F0(Iter801169D0 *it);
s32 func_800B0808(Iter801169D0 *it);
Item801169D0 *func_800B0864(Iter801169D0 *it);
void func_80115780(Item801169D0 *item);
s32 func_801157E4(Item801169D0 *item);
void func_800E1048(void *arg0);

void func_801169D0(void) {
    Iter801169D0 it;

    if (--D_801486E4 != 0) {
        return;
    }
    func_80049CB4(0x131);
    func_800B07F0(&it);
    while (func_800B0808(&it)) {
        Item801169D0 *item = func_800B0864(&it);

        if (item->kind_0 == 0x10) {
            func_80049CB4(6);
            func_80115780(item);
            func_80049CB4(7);
        }
    }
    func_80049CB4(2);
    it.index_4 = 0;
    while (func_800B0808(&it)) {
        Item801169D0 *item = func_800B0864(&it);

        if (item->kind_0 == 0x10) {
            func_801157E4(item);
        }
    }
    D_801486DC = 0;
    D_801486E0 = 0;
    D_801486E4 = 0;
    func_800E1048(D_801476B8);
}
