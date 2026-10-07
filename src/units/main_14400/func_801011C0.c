#include "common.h"
typedef unsigned char u8;
typedef struct { char pad[0x1E]; unsigned char x1E; } Ent;
typedef struct { char pad[0x58]; void *x58; } Unit;
extern char D_80147620[];
u8 func_800A6420(void *p, void *v);
s32 func_800F1024(Unit *p);
u8 func_800C57A0(void *rng);
void func_800A665C(Unit *p, unsigned char *dir);
Ent *func_800A6BA4(Unit *p, s32 kind, s32 flags);
s32 func_800A44F4(Unit *p, Ent *e);
s32 func_800E1CC4(Ent *e, s32 kind);
void func_800A2F80(unsigned char *dir, s32 step);
s32 func_800E1CD4(Unit *p, s32 kind);
s32 func_800E7104(Unit *p);
void func_800F06E4(Unit *p);
s32 func_800E8350(Unit *p);
s32 func_801011C0(Unit *p) {
    unsigned char dir;
    s32 i;
    if (func_800A6420(p, p->x58) != 3 && func_800F1024(p)) {
        dir = func_800C57A0(D_80147620) & 7;
        i = 0;
        for (;;) {
            Ent *e;
            s32 found;
            if (i >= 8) break;
            func_800A665C(p, &dir);
            e = func_800A6BA4(p, 0x4C, 0);
            found = e != 0 && (e->x1E & 0x7C) && func_800A44F4(p, e) == 1 && !func_800E1CC4(e, 1);
            if (found) {
                p->x58 = 0;
                func_800F06E4(p);
                return 0;
            }
            func_800A2F80(&dir, 1);
            i++;
        }
    }
    if (func_800E1CD4(p, 0x10)) return func_800E8350(p);
    return func_800E7104(p);
}
