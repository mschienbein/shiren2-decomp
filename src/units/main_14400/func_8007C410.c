#include "common.h"
typedef struct {
    short field00, kind; unsigned char field04; signed char mode;
    unsigned char field06, state, field08, phase; unsigned short flags;
    unsigned char pad0C[0xA4];
} Actor;
extern Actor D_801DEAB4[30];
extern s32 D_8013D8CC;
/* Returns -1/0 status; this caller discards it. */
extern s32 func_800751B4(s32 id, s32 flags);
extern s32 func_80076044(s32 id, s32 animation, s32 mode, s32 command, s32 flags);
/* Original returns -1 when disabled/unsupported and 0 after processing. */
s32 func_8007C410(s32 kind, s32 mode) {
    s32 index;
    if (!D_8013D8CC || (kind != 0x17 && kind != 0x1B)) return -1;
    for (index = 0; index < 30; index++) {
        Actor *actor = &D_801DEAB4[index];
        if (actor->kind != -1 && actor->kind == kind &&
            (!(actor->flags & 0xBFFF) || actor->mode == 1) &&
            actor->phase >= 3 && actor->state != 2 && actor->mode != mode) {
            s32 animation;
            s32 flags;
            actor->mode = mode;
            if (mode == 1) {
                switch (actor->kind) {
                case 0x17: animation = 0x185; flags = 3; break;
                case 0x1B: animation = 0x15B; flags = 0; break;
                default: animation = 0; flags = 3; break;
                }
            } else {
                switch (actor->kind) {
                case 0x17: animation = 0; flags = 3; break;
                case 0x1B:
                    func_800751B4(index, actor->flags & 0x4000);
                    animation = 0; flags = 3; break;
                default: animation = 0; flags = 3; break;
                }
            }
            func_80076044(index, animation, 1, 8, flags);
        }
    }
    return 0;
}
