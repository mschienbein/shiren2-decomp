#include "func_801ECC10_13EB30/types.h"

extern Actor801476B8 *D_801476B8;
extern World801ECC10 *D_80142B10;
extern SelectionRecord D_80142F18;
extern u8 D_80140160[];
s32 func_80049CB4(s32 id, ...);
void func_801E8634_13A554(s32, s32, s32);
void func_80046C30(s32);
int func_800413E0(void);
void func_800B123C(void);
s32 func_800A251C(Vec2i *x, Vec2i *y);
void func_800A58FC(void *actor, Vec2i *pos);
void func_801E5874_137794(void);
void func_80045E50(void);
void func_801ECE10_13ED30(u8 mode, Vec2i *pos);
void func_800B6F58(void *unused);
void func_801F1914_143834(s32);
void func_801F1FF4_143F14(void);
void func_800C77C8(void);
void func_800C7830(void);
void func_800C92BC(void *actor);
void func_801EEA38_140958(void);
void func_801F4FE0_146F00(s32);
void func_80094B80(u8 *system, s32);
void func_80122600(void);
void func_800413FC(void);
void func_800D7798(void);
void func_800AA944(void);

static inline s32 controls_disabled(SelectionRecord *record) {
    return (record->flags >> 2) & 1;
}

s32 func_801ECC10_13EB30(u8 kind, u8 mode, Vec2i *pos) {
    Vec2i origin;
    World801ECC10 *world;

    origin.x = 0;
    origin.y = 0;
    func_80049CB4(2);
    func_80049CB4(0x87, D_801476B8);
    func_80049CB4(2);
    func_801E8634_13A554(0, 0, 0);
    func_80046C30(2);
    func_800413E0();
    func_800B123C();
    world = D_80142B10;
    if (func_800A251C(pos, &origin)) {
        world->vtable->fn20((u8 *)world + world->vtable->delta20);
    } else {
        func_800A58FC(D_801476B8, pos);
    }
    func_801E5874_137794();
    func_80045E50();
    func_80049CB4(2);
    D_801476B8->b52 = 1;
    if (func_800A251C(pos, &origin)) {
        func_801ECE10_13ED30(mode, &origin);
        func_800B6F58(world);
    } else {
        func_801ECE10_13ED30(mode, &D_801476B8->pos);
    }
    if (D_801F51E8_147108->b9F6 == 0) {
        func_801F1914_143834(1);
    }
    func_801F1FF4_143F14();
    func_800C77C8();
    func_800C7830();
    func_800C92BC(D_801476B8);
    func_801EEA38_140958();
    if (controls_disabled(&D_80142F18)) {
        func_801F4FE0_146F00(0);
        func_80094B80(D_80140160, 0);
    } else {
        func_801F4FE0_146F00(1);
        func_80094B80(D_80140160, 1);
    }
    func_80122600();
    func_800413FC();
    func_800D7798();
    if (D_801F51E8_147108->b9F9 == 0) {
        func_800AA944();
    }
    return 4;
}
