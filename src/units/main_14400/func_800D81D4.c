#include "common.h"
typedef struct { char pad[0xA]; unsigned char unkA; char padB[0x9A - 0xB]; unsigned short unk9A; char pad9C[1]; unsigned char unk9D; } Unit;
typedef struct { unsigned char unk0; unsigned char unk1; char pad2[0xA]; unsigned char unkC; unsigned char unkD; unsigned char unkE; unsigned char unkF; } Thing;
typedef struct { s32 cur; } UnitIter;
typedef struct { s32 a; s32 b; } ThingIter;
s32 func_8004505C(s32 id, void *outKind, void *outLevel);
s32 func_800D81C0(s32 id);
s32 func_800A8FC8(UnitIter *, s32);
Unit *func_800A910C(UnitIter *);
s32 func_800E0F40(Unit *);
void *func_800B07F0(void *iter);
s32 func_800B0808(ThingIter *);
Thing *func_800B0864(ThingIter *);
s32 func_800D81D4(s32 id) {
    ThingIter things;
    unsigned char kind;
    unsigned char sub;
    UnitIter units;
    s32 failed = func_8004505C(id, &kind, &sub) != 1;
    if (failed) return -1;
    if (func_800D81C0(id) != -1) {
        units.cur = 0;
        while (func_800A8FC8(&units, 0x10)) {
            Unit *u = func_800A910C(&units);
            s32 found = 0;
            if ((u->unk9A & 0x200) && u->unkA == kind) found = (unsigned char)func_800E0F40(u) == sub;
            if (found) return u->unk9D;
        }
        func_800B07F0(&things);
        while (func_800B0808(&things)) {
            Thing *t = func_800B0864(&things);
            s32 found = 0;
            if (t->unk1 == 0xF1 && (t->unkC & 4) && t->unkD == kind) found = t->unkE == sub;
            if (found) return t->unkF;
        }
    }
    return func_800D81C0(id);
}
