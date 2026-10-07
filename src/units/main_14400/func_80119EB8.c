#include "common.h"
typedef struct { char pad[8]; short field_8; void (*field_c)(void *self, s32 flags); } Methods;
typedef struct { char pad[8]; Methods *field_8; unsigned char field_c; } Obj;
typedef struct { char pad0[0xA]; unsigned char field_a; char padb[0x13]; unsigned char field_1e; } Other;
typedef struct { s32 type,arg; Other *other; } Event;
extern s32 func_800A692C(Other*,s32),func_800E1CD4(Other*,s32),func_801131F8(Obj*,Event*);
extern void func_80119AE4(Obj*,s32,Other*),func_800D3650(Obj*);
static inline s32 get_flag(Obj *p) { return p->field_c&1; }
static inline s32 below(s32 v,s32 limit) { return v<limit; }
s32 func_80119EB8(Obj *p,Event *event) {
    if(below(event->type,20) && !below(event->type,18)) {
        Other *other=event->other;
        s32 eligible=(other->field_1e&0x7C) && !func_800A692C(other,10) && !func_800E1CD4(other,15) && other->field_a!=0x5D && !get_flag(p);
        if(eligible) {
            func_80119AE4(p,event->arg,other);
            if(event->type==18) { func_800D3650(p); if(p) p->field_8->field_c((char*)p+p->field_8->field_8,3); }
            return 1;
        }
    }
    return func_801131F8(p,event);
}
