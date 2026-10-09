#include "common.h"
typedef struct Obj Obj;
typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VtEntry;
typedef struct { char pad[0x18]; VtEntry *vt; } Target;
extern char D_8015CECC[];
void func_800AF11C(void *, Target *);
void func_800CA4A4(Target *, void *);
void func_8010C3C4(char *self, Target *t){
    func_800AF11C(self, t);
    func_800CA4A4(t, D_8015CECC);
    t->vt[3].fn((char *)t + t->vt[3].delta, 0x14, self + 0xC);
}
