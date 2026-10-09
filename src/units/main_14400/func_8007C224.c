#include "common.h"

typedef struct { char pad[2]; short kind; char rest[0xAC]; } Ent;
typedef struct { signed char active; char rest[45]; } Info;
extern Ent D_801DEAB4[]; extern Info D_801A79E8[];
/* Returns -1/0 status; this caller discards it. */
s32 func_8007C410(s32 kind, s32 mode);
void func_8007C224(void){
    Ent *p;
    Ent *end;
    Info *info;
    s32 kind;
    p = D_801DEAB4;
    end = &D_801DEAB4[29];
    for (; p <= end; p++) {
        info = &D_801A79E8[p - D_801DEAB4];
        kind = p->kind;
        if (kind != -1 && info->active != 0 && (kind == 0x17 || kind == 0x1B))
            func_8007C410(kind, 0);
    }
}
