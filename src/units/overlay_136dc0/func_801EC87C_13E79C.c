#include "common.h"
/*
 * func_801EC87C_13E79C (opus-01, Anthropic claude-opus-5-5 xhigh; base: opus-08 t2/f1_legal_best.c).
 *
 * The third parameter is the u8 mode that the only caller passes (func_801EDFBC_13FEDC's u8
 * `first`) and that is only forwarded to func_801ECC10_13EB30(u8 kind, u8 mode, ...). As a
 * promoted u8 parameter, gcc 2.8.1 copies a2 in assign_parms' deferred conversion insn,
 * which (unlike the plain hard-register entry copies) sched1 may move into the load-use stall
 * of the reloaded 0x10C word (ROM daddu s3,a2 at 0x801EC8B8). The byte narrowing for the call stays at
 * the use (ROM andi a1,s3,0xFF at 0x801ECB88).
 */

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct D_801F51E8_s {
    u8 pad_000[0x10C];
    s32 x10C;
    s32 x110;
    u8 pad_114[0x9F6 - 0x114];
    u8 b9F6;
    u8 pad_9F7[0xA04 - 0x9F7];
} D_801F51E8_s;

extern D_801F51E8_s *D_801F51E8_147108;

typedef struct { s32 x; s32 y; } Vec2i;
typedef struct { s32 unk0; s32 unk4; } Iterator801EC87C;

extern u8 D_801F5CCE_147BEE;
extern s32 D_801F5D04_147C24;
extern s32 D_80147678;
void func_801E9F90_13BEB0(s32, u8);
u8 func_801E9F70_13BE90(s32);
void func_801E5558_137478(s32);
void func_801E7850_139770(void);
void func_801F356C_14548C(void);
void func_801E6688_1385A8(s32);
void func_801E6484_1383A4(void);
void func_801F3544_145464(void);
void func_801E71B8_1390D8(void);
void func_801E7414_139334(void);
void func_801E7AD8_1399F8(void);
void func_801E6968_138888(void);
void func_801E665C_13857C(void);
void func_801E7FB4_139ED4(void);
void func_801E700C_138F2C(void);
void func_801E8170_13A090(void);
void func_801E8A68_13A988(void);
void func_800B75E8(Vec2i *pos);
void func_801F4FE0_146F00(s32);
s32 func_80049CB4(s32 id, ...);
s32 func_800A8F6C(Iterator801EC87C *it);
void *func_800A910C(Iterator801EC87C *it);
void func_801ECECC_13EDEC(void);
void func_800B4B88(void);
void func_800B74D8(void);
void func_800B1504(void);
void func_800A880C(void);
void func_80048380(void);
void func_80045A84(void);
void func_801E7C34_139B54(void);
void func_801E8758_13A678(s32, s32, s32);
void func_800AABEC(void);
void func_801E8A0C_13A92C(void);
void func_800AA2BC(u8, u8, u8);
void func_801E7C50_139B70(void);
int func_800413E0(void);
void func_800B123C(void);
void func_801E5874_137794(void);
void func_800413FC(void);
void func_800A9C8C(u8, u8);
s32 func_801ECC10_13EB30(u8 kind, u8 mode, Vec2i *pos);

/* Inline coordinate setter: both arguments are loaded before either store
 * (ROM 0x13E7A0/0x13E7A4 loads, 0x13E7CC/0x13E7D0 stores). */
static inline void set_pos(Vec2i *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}

s32 func_801EC87C_13E79C(s32 kind, s32 arg1, u8 mode) {
    Vec2i pos;
    Vec2i origin;
    Iterator801EC87C it;
    s32 i;
    s32 value;

    /* Original stores: x,y at ROM 0x13E7CC/0x13E7D0, then the reloaded y,x at
     * ROM 0x13E7DC/0x13E7E8. Both pairs are real instructions in the ROM. */
    set_pos(&pos, D_801F51E8_147108->x110, D_801F51E8_147108->x10C);
    pos.y = D_801F51E8_147108->x10C;
    pos.x = D_801F51E8_147108->x110;
    for (i = 0x18; i < 0x22; i++) {
        func_801E9F90_13BEB0(i, 0);
    }
    if (func_801E9F70_13BE90(0x39)) {
        D_801F51E8_147108->b9F6 = 1;
    }
    func_801E9F90_13BEB0(0x39, 0);
    if (kind == 4) {
        return 0xB;
    }
    if (kind == 6) {
        return 0xC;
    }
    func_801E5558_137478(0);
    func_801E7850_139770();
    func_801F356C_14548C();
    func_801E6688_1385A8(0);
    func_801E6484_1383A4();
    func_801F3544_145464();
    func_801E71B8_1390D8();
    func_801E7414_139334();
    func_801E7AD8_1399F8();
    func_801E6968_138888();
    func_801E665C_13857C();
    func_801E7FB4_139ED4();
    func_801E700C_138F2C();
    func_801E8170_13A090();
    func_801E8A68_13A988();
    if ((kind & 0xE0) == 0xE0) {
        origin.x = 0;
        origin.y = 0;
        func_800B75E8(&origin);
        value = 1;
    } else {
        value = 0;
    }
    func_801F4FE0_146F00(value);
    func_80049CB4(2);
    it.unk0 = 0;
    while (func_800A8F6C(&it)) {
        func_80049CB4(0x87, func_800A910C(&it));
    }
    func_80049CB4(2);
    if (kind != 3) {
        if ((kind & 0xE0) == 0xE0) {
            func_801ECECC_13EDEC();
        } else {
            func_800B4B88();
            func_800B74D8();
        }
    }
    func_800B1504();
    func_800A880C();
    func_80048380();
    func_80049CB4(2);
    D_801F5CCE_147BEE = 1;
    if (kind == 2) {
        func_80045A84();
        func_801E7C34_139B54();
        func_801E8758_13A678(0, 0, 0);
        func_800AABEC();
        func_801E8A0C_13A92C();
    } else if (kind == 0x75) {
        func_800AA2BC(0x75, arg1, 0xFF);
        if (D_801F5D04_147C24 == 1) {
            func_801E7C50_139B70();
            func_800413E0();
            func_800B123C();
        }
        func_801E5874_137794();
        func_80049CB4(2);
        func_800413FC();
        func_80049CB4(2);
    } else if ((kind & 0xE0) == 0xE0 || kind == 3) {
        D_801F5CCE_147BEE = 0;
        func_800A9C8C(kind & 0x1F, arg1);
        D_80147678 = 10;
        func_801E9F90_13BEB0(0xE, 0);
    } else {
        func_800AA2BC(kind, arg1, 0xFF);
        func_801ECC10_13EB30(kind, mode, &pos);
    }
    func_80049CB4(2);
    if (kind == 3) {
        return 10;
    }
    if ((kind & 0xE0) == 0xE0) {
        return 8;
    }
    return 4;
}
