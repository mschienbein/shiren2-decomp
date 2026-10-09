#include "common.h"
typedef unsigned short u16;
typedef short s16;
/* Scheduler record handler: called with its own 0x74-byte record as receiver. */
typedef void (*Handler)(void *);
typedef struct { Handler field0; u16 field4; u16 field6; char pad8[6]; u16 fieldE; u16 field10; u16 field12; s32 field14; char pad18[0xC]; s32 field24; s32 field28; char pad2C[0x48]; } Obj;
typedef struct { s16 field0; s16 field2; char pad4[0x38]; u16 field3C; } Info;
extern Obj D_801BA380[];
extern void func_800855E0(void *), func_80085C24(void *), func_8008B678(void *);
extern Info *func_8007946C(s32, s32);
extern s32 func_801E6AFC(s32);
extern void func_8008B8A8(s32, s32, s32, s32);
void func_8008B9B0(Obj *p) {
    Info *info; s32 i;
    if (p->field10 != p->field6) {
        Obj *entries = D_801BA380;
        s32 found;
        if (entries[p->field10].field0 == func_800855E0) return;
        found = 0;
        for (i = p->field6 - 1; i >= 0; --i) { Obj *entry = &entries[i]; if (entry->field0 == func_80085C24 && !(entry->field12 & 4) && p->field14 == entry->field24 && entry->field4 != 4) { ++found; break; } }
        if (found) return;
    }
    for (i = 0; i < p->field6; ++i) { Obj *entry = &D_801BA380[i]; if (entry->field0 == p->field0 && entry->field14 == p->field14) entry->field0 = func_8008B678; }
    info = func_8007946C(0, p->field14);
    if (info->field2 != -1 && func_801E6AFC(p->field14)) func_8008B8A8(p->field14, p->field24, p->field28, info->field3C);
    p->fieldE = 1; p->field4 = 4;
}
