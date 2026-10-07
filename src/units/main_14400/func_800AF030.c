#include "common.h"
typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VtEntry;
typedef struct { char pad[0x18]; VtEntry *vt; } Obj;
extern char D_80153A38[];
extern char D_80148720[], D_801485F0[], D_80148650[], D_801485B0[], D_80148690[], D_80148470[];
void func_800CA4E8(Obj *, void *);
#define VCALL5(o, n, s) (o)->vt[5].fn((char *)(o) + (o)->vt[5].delta, n, s)
void func_800AF030(Obj *o){
    func_800CA4E8(o, D_80153A38);
    VCALL5(o, 0x16, D_80148720);
    VCALL5(o, 0x1B, D_801485F0);
    VCALL5(o, 0x13, D_80148650);
    VCALL5(o, 0x13, D_801485B0);
    VCALL5(o, 0xD, D_80148690);
    VCALL5(o, 0x15, D_80148470);
}
