#include "common.h"
/* Whole 0x70-byte object, including the member constructed at +0x18. */
typedef struct SharedMenu {
    s32 index; void *records; s32 unknown_08; void *vtable_0C;
    s32 active_10; s32 flag_14;
    struct {
        s32 unknown_00[3]; s32 value_0C;
        s32 unknown_10[3]; s32 value_1C;
        s32 unknown_20[3]; const void *vtable_2C;
        s32 unknown_30[3]; const void *vtable_3C;
        s32 unknown_40[6];
    } member_18;
} SharedMenu;
extern SharedMenu D_80140080;
extern char D_80151350[], D_80151400[], D_801513B0[];
extern const unsigned char D_80151480[24];
void func_80092038(void) {
    SharedMenu *g = &D_80140080;
    g->vtable_0C = D_80151350;
    g->records = D_80151400;
    g->vtable_0C = D_801513B0;
    g->member_18.value_0C = -1;
    g->member_18.value_1C = -1;
    g->member_18.vtable_2C = D_80151480;
    g->member_18.vtable_3C = D_80151480;
}
