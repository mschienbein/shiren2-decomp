#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { char pad[0x1E]; u8 flags1E; } Thing;
typedef struct {
    Pos pos;
    char pad8[0x20];
    s16 unk28;
} Unit;
typedef struct {
    Thing *source;
    s32 type;
    char pad8[6];
    u16 flagsE;
} Attack;
extern u8 D_801531A0[];
extern void *D_801476B8;
extern u8 D_80156A09;
extern u8 D_80156A79;
void func_800A59A4(Unit *);
s32 func_800E4454(Unit *);
void func_800E4470(Unit *);
char *func_800A3B20(void *actor);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32, ...);
s32 func_800A44F4(void *, void *);
void *func_800C5F60(void);
u8 func_800A6420(void *actor, void *other);
void func_800497F0(s32, ...);
s32 func_800A08D8(s32, s32, s32);
void func_80049AE8(s32, ...);
s32 func_800B5300(Pos *pos, void *arg, u8 mode);
s32 func_800E41EC(Unit *, Attack *);
s32 func_800A692C(Unit *, s32);
s32 func_800A58B8(Unit *);
void func_800A00C4(Pos *, u8, Unit *, s32);
s32 func_800E20CC(Unit *);
void func_80049C90(s32, s32);
static inline void Pos_set(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
s32 func_800E3D20(Unit *self, Attack *attack) {
    Pos pos;
    Thing *source;
    s32 text;
    s32 named;
    s32 selfBlind;
    s32 sourceBlind;
    s32 done;
    char *name;

    self->unk28 = 0;
    func_800A59A4(self);
    if (func_800E4454(self) && attack->type != 0x15) {
        if (attack->type != 0x1E) {
            func_800E4470(self);
        }
        if (attack->source != 0 && ((attack->source->flags1E >> 2) & 1)) {
            func_800498E4(0x226, func_800A3B20(self));
        }
    }
    source = attack->source;
    Pos_set(&pos, &self->pos);
    text = func_80049CB4(0xDA, &pos);
    selfBlind = 0;
    sourceBlind = selfBlind;
    named = text != -2;
    if (D_801531A0[attack->type] & 0x10) {
        text = -2;
    } else if (func_800A44F4(D_801476B8, self) == 1) {
        text = -1;
        selfBlind = 1;
    } else if (func_800A44F4(D_801476B8, source) == 1) {
        text = -1;
        sourceBlind = 1;
    } else {
        s32 hidden = 0;
        if ((attack->flagsE >> 5) & 1) {
            hidden = func_800A6420(source, func_800C5F60()) != 3;
        }
        if (hidden) {
            text = -1;
        }
    }
    if (!(D_801531A0[attack->type] & 8)) {
        func_80049CB4(0x1131);
        func_80049CB4(6);
        func_80049CB4(0x70, self);
        func_80049CB4(7);
    }
    if (text != -2) {
        name = func_800A3B20(self);
        func_80049CB4(6);
        if (attack->type == 0x22) {
            func_800497F0(0x106, text, name);
            func_800A08D8(1, text, 0);
        } else if (attack->type != 0xA) {
            if (source == 0) {
                func_800497F0(0x49, text, name);
            } else {
                char *sourceName = func_800A3B20(source);
                if (source == self) {
                    func_800497F0(0x4A, text, name);
                } else if (selfBlind) {
                    if (named) {
                        func_800497F0(0x49, text, name);
                    } else {
                        func_800497F0(0x48, text, name, sourceName);
                    }
                } else if (sourceBlind) {
                    if (named) {
                        func_80049AE8(0x45, text, name);
                    } else {
                        func_80049AE8(0x46, text, sourceName, name);
                    }
                } else {
                    func_800497F0(0x47, text, sourceName, name);
                }
            }
        }
        func_80049CB4(7);
    }
    if (attack->type == 0x22) {
        func_80049CB4(6);
        func_800B5300(&pos, 0, D_80156A09);
        func_800497F0(0x107, text);
        func_80049CB4(7);
    }
    if (D_801531A0[attack->type] & 0x20) {
        func_80049CB4(0x132);
        return 0;
    }
    if (func_800E41EC(self, attack)) {
        func_80049CB4(6);
        func_80049CB4(0x120, &pos);
        func_80049CB4(7);
    }
    done = 0;
    if (func_800A692C(self, 8)) {
        if (func_800A58B8(self) == 1) {
            func_80049CB4(6);
            func_80049CB4(0x10B, &pos);
            func_80049CB4(7);
            func_80049CB4(0xF0, &pos);
        } else {
            func_80049CB4(0x115, &pos);
            func_800497F0(0x22B, text, func_800A3B20(self));
            func_80049CB4(0x132);
            if (!(D_801531A0[attack->type] & 8)) {
                done = func_800A08D8(1, text, 0);
            }
            func_800A00C4(&pos, D_80156A79, self, 3);
        }
    }
    if (func_800E20CC(self)) {
        func_80049CB4(0x89, self);
    }
    func_80049CB4(0x132);
    if (!done) {
        if (!(D_801531A0[attack->type] & 8)) {
            done = func_800A08D8(1, text, 1);
        }
        if (!done && selfBlind) {
            func_80049C90(1, text);
        }
    }
    func_80049CB4(0x132);
    return 1;
}
