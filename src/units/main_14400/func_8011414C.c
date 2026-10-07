#include "common.h"
/* The destructor's virtual methods also use ids_8, capacity_C and count_E. */
typedef struct {
    void *owner;
    void *vt;
    unsigned char *ids_8;
    unsigned char capacity_C;
    unsigned char unknown_D;
    unsigned char count_E;
} Sub;
extern char D_8015D938[], D_80153AA0[];
extern void func_8011541C(void *);
extern void func_800CE6A0(Sub *, s32);
extern void func_800AC68C(void *);
typedef struct { char pad[8]; void *vt; Sub sub; } Obj;
void func_8011414C(Obj *self, s32 flags){ self->vt = D_8015D938; func_8011541C(self); func_800CE6A0(&self->sub, 2); self->vt = D_80153AA0; if (flags & 1) func_800AC68C(self); }
