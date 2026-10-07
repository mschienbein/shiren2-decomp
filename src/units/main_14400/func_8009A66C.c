#include "common.h"
typedef unsigned char u8;
typedef struct { u8 kind; } Def;
/* The 0x60 subobject spans 0x1C bytes; its child begins at +4. */
typedef struct { char pad0[0x5C]; Def *def; char v60[0x1C]; s32 v7C; } S;
extern void *D_801476B8;
extern void *func_800E8A68(void *obj, u8 arg1);
extern s32 func_8010C38C(void *);
extern void func_80095224(void *obj, void *item, void *def);
extern void func_80048728(void *);
void func_8009A66C(S *p, s32 v) {
    void *it;
    if (v == p->v7C) return;
    if (p->def == 0) return;
    if (v != 0) {
        it = func_800E8A68(D_801476B8, p->def->kind);
        if (it == 0) {
            switch (p->def->kind) {
            case 3:
                it = func_800E8A68(D_801476B8, 4);
                break;
            case 4:
                it = func_800E8A68(D_801476B8, 3);
                break;
            }
            if (it != 0) {
                s32 usable = func_8010C38C(it) == 1;
                if (!usable) it = 0;
            }
        }
        func_80095224(p->v60, it, p->def);
    } else {
        func_80048728(p->v60 + 4);
    }
    p->v7C = v;
}
