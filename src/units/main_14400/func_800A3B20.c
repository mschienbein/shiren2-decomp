#include "common.h"
typedef unsigned short u16;
typedef struct { char pad[0x30]; short d30; short i30; char *(*describe)(void *, char *);
                 char pad38[0x90 - 0x38]; short d90; short i90; s32 (*check)(void *, s32, s32, unsigned char, s32); } UnitVT;
typedef struct {
    char pad0[0x1E];
    unsigned char f1E;
    unsigned char f1F;
    char pad20[0x24 - 0x20];
    UnitVT *vt;
    char pad28[0x75 - 0x28];
    unsigned char f75;
    char pad76[0xE4 - 0x76];
    unsigned short fE4;
} Unit;
extern unsigned char D_801429E4;
extern char D_801C3590[][20];
extern Unit *D_801476B8;
s32 func_800E1CC4(Unit *u, s32 v);
s32 func_800E1CD4(Unit *u, s32 v);
char *func_80048480(u16 id);
s32 func_800E4454(Unit *u);
char *func_800A8498(s32 a, s32 b);
char *func_80083C90(char *buf, char *msg);
static inline s32 Unit_isAsleep(Unit *u) { return (u->f1E >> 2) & 1; }
static inline s32 Unit_flag3(Unit *u) { return (u->fE4 >> 3) & 1; }
char *func_800A3B20(Unit *u) {
    char *buf;
    D_801429E4 = (D_801429E4 + 1) & 3;
    buf = D_801C3590[D_801429E4];
    if (u->f1E & 0x7C) {
        s32 notAsleep = Unit_isAsleep(u) != 1;
        if (notAsleep) {
            s32 flag;
            if (func_800E1CC4(D_801476B8, 0)) {
                func_80083C90(buf, func_80048480(0x231));
                return buf;
            }
            if (func_800E1CD4(u, 15)) {
                func_80083C90(buf, func_80048480(0x234));
                return buf;
            }
            flag = 0;
            if (func_800E1CC4(u, 1)) {
                flag = Unit_flag3(D_801476B8) == 0;
            }
            if (flag) {
                func_80083C90(buf, func_80048480(0x231));
                return buf;
            }
        }
        if (func_800E1CD4(u, 15)) {
            func_80083C90(buf, func_80048480(0x234));
            return buf;
        }
        if (u->vt->check((char *)u + u->vt->d90, 2, 9, 0, 0)) {
            func_80083C90(buf, func_80048480(0x80D));
            return buf;
        }
        if (func_800E4454(u)) {
            func_80083C90(buf, func_800A8498(u->f1F, u->f75));
            return buf;
        }
    }
    return u->vt->describe((char *)u + u->vt->d30, buf);
}
