#include "common.h"
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct {
    Pos pos;
    char pad8[0x1E - 0x8];
    unsigned char f1E;
    char pad1F[0x5C - 0x1F];
    Pos f5C;
    char pad64[0xE4 - 0x64];
    unsigned short fE4;
    char padE6[0x100 - 0xE6];
    s32 f100;
} Unit;
typedef struct { s32 index; s32 f4; } UnitIter;
typedef struct { unsigned char f0; } ItemHead;
extern Unit *D_801476B8;
extern unsigned short D_8014767C;
extern s32 D_8013960C;
extern unsigned short D_801476C0;
extern s32 D_80147678;
extern short D_80147660;
extern unsigned char D_801476BC;
extern s32 D_80148090;
extern unsigned short D_801F5D0E;
extern unsigned short D_801476BE;
extern short D_80158C6C[];
s32 func_80049CB4(s32 id, ...);
void func_801F4B74(void);
void func_801F4D10(void);
Unit *func_800C5F60(void);
s32 func_800E04D0(Unit *u);
void func_8004839C(void);
void func_800498E4(s32 v, ...);
void func_800C8194(void);
s32 func_800B60E0(void);
s32 func_800C81E4(Unit *u);
void func_800C92F8(Unit *u);
void func_800C9500(void);
void func_800B57F4(void);
void func_800B5D54(void);
s32 func_800A8FC8(UnitIter *it, s32 kind);
Unit *func_800A910C(UnitIter *it);
void func_800E4DE8(Unit *u);
void func_800EBFE4(Unit *u);
void func_800C8440(unsigned short speed, unsigned short turns);
void func_8011BFF8(void);
u32 func_80048AC8(void);
void func_80046BF4(s32 a, s32 b);
void func_800EC208(Unit *u);
void func_800C86C8(Unit *u, s32 result, short turns);
void func_800C878C(short turns);
void func_800C8868(s32 v);
void func_800C8EE8(void);
ItemHead *func_800A6DC8(Unit *u);
void func_800C8FAC(void);
void func_800C8A38(u16 turns);
void func_800C8C18(u16 turns);
void func_800B5E88(void);
void func_800C90C0(void);
void func_80045E50(void);
void func_800D37E8(s32 v);
static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static inline s32 Unit_speed(Unit *u) { return D_80158C6C[(unsigned char)func_800E04D0(u)]; }
static inline s32 Unit_isAsleep(Unit *u) { return (u->f1E >> 2) & 1; }
static inline s32 flagBit(u32 flags, s32 bit) { return (flags >> bit) & 1; }
s32 func_800C7BA4(void) {
    Unit *u;
    short speed;
    short prevSpeed;
    s32 result;
    s32 turns;
    s32 isPlayer;
    s32 extra;
    Pos pos;
    func_80049CB4(1);
    func_80049CB4(3);
    func_80049CB4(10);
    func_801F4B74();
    D_8014767C &= ~0x40;
    D_801476B8->fE4 &= ~0x80;
    u = func_800C5F60();
    speed = Unit_speed(u);
    if (speed >= 121) {
        s32 notFast = flagBit(D_8014767C, 7) != 1;
        speed = 120;
        if (notFast) {
            D_8014767C = (D_8014767C | 0x80) & ~0x100;
        }
    } else {
        D_8014767C &= ~0x180;
    }
    D_8013960C = 1;
    func_8004839C();
    if (D_801476C0 != 0) {
        func_800498E4(D_801476C0);
        func_80049CB4(0x1129, 0x14);
        func_80049CB4(2);
        D_801476C0 = 0;
    }
    func_800C8194();
    func_800B60E0();
    Pos_copy(&pos, &u->pos);
    result = func_800C81E4(u);
    u = func_800C5F60();
    func_800C92F8(u);
    func_800C9500();
    if (result != 2) {
        func_80049CB4(2);
    } else {
        func_80049CB4(0x11, 0);
    }
    if (D_80147678 != 0) {
        return D_80147678;
    }
    switch (result) {
    case 1:
        return 0;
    case 4:
        return 12;
    case 5:
        return 4;
    case 6:
        return 14;
    }
    prevSpeed = speed;
    func_800B57F4();
    u->f5C = pos;
    func_800C92F8(u);
    func_800B5D54();
    func_80049CB4(4);
    func_80049CB4(3);
    speed = Unit_speed(u);
    if (speed >= 121) {
        speed = 120;
    } else if (speed < prevSpeed) {
        UnitIter it;
        it.index = 0;
        while (func_800A8FC8(&it, 0x7C)) {
            Unit *m = func_800A910C(&it);
            s32 awake = Unit_isAsleep(m) != 1;
            if (awake) {
                func_800E4DE8(m);
            }
        }
    }
    turns = 0;
    for (D_80147660 -= speed; D_80147660 <= 0; D_80147660 += 120) {
        turns++;
    }
    isPlayer = 0;
    if (result == 2) {
        isPlayer = u == D_801476B8;
    }
    if (isPlayer) {
        func_800EBFE4(D_801476B8);
    }
    func_800C8440(speed, turns);
    if (isPlayer) {
        D_801476B8->f100 = 0;
    }
    func_800C9500();
    extra = 0;
    func_80049CB4(2);
    func_80049CB4(10);
    func_80049CB4(4);
    if (D_801476BC & 1) {
        extra = 1;
        func_80049CB4(9);
        func_8011BFF8();
        func_80049CB4(10);
    }
    if (func_80048AC8()) {
        func_80046BF4(D_80148090 != 0, extra);
        if (extra) {
            func_80049CB4(2);
        }
    }
    func_800B60E0();
    if (result == 2 && u == D_801476B8) {
        func_800EC208(u);
    }
    func_800C86C8(u, result, turns);
    func_800C92F8(u);
    func_80049CB4(0x89, D_801476B8);
    func_80049CB4(2);
    if (D_80147678 != 0) {
        return D_80147678;
    }
    func_800C878C(turns);
    if (D_80147678 != 0) {
        return D_80147678;
    }
    func_800C8868(result != 2);
    if (D_80147678 != 0) {
        return D_80147678;
    }
    func_800C8EE8();
    func_800C92F8(u);
    func_800C9500();
    func_80049CB4(0x89, D_801476B8);
    func_80049CB4(2);
    if (func_800A6DC8(D_801476B8) != 0 && result == 2) {
        if (func_800A6DC8(D_801476B8)->f0 == 0xF) {
            D_80148090 = 0;
        }
    }
    func_800C8FAC();
    if (D_80147678 != 0) {
        return D_80147678;
    }
    func_800C8A38(turns);
    if (D_80147678 != 0) {
        return D_80147678;
    }
    func_800C8C18(turns);
    func_80049CB4(0x89, D_801476B8);
    func_800C9500();
    func_800B5E88();
    D_8013960C = 0;
    func_801F4D10();
    func_800C90C0();
    if (D_801F5D0E == 0) {
        func_80049CB4(0x139);
    }
    func_80049CB4(2);
    func_80049CB4(10);
    func_80049CB4(4);
    func_80045E50();
    func_800D37E8(0);
    {
        unsigned short flags = D_8014767C;
        if (flagBit(flags, 7)) {
            if (flagBit(flags, 8)) {
                D_8014767C = flags & ~0x100;
            } else {
                D_8014767C = flags | 0x100;
            }
        }
    }
    D_801476BE++;
    return 0;
}
