#include "common.h"
#include "item_records.h"
typedef struct { char pad0[0xA]; unsigned char field_a; char padb[0x13]; unsigned char field_1e; } Other;
typedef struct { s32 type; void *arg; Other *other; } Event;
extern s32 func_800A692C(Other*,s32),func_800E1CD4(Other*,s32),func_801131F8(Item*,Event*);
extern void func_80119AE4(Item*,void*,Other*),func_800D3650(Item*);
static inline s32 get_flag(Item *p) { return p->payload.consumable.flags0C&1; }
static inline s32 below(s32 v,s32 limit) { return v<limit; }
s32 func_80119EB8(Item *p,Event *event) {
    if(below(event->type,20) && !below(event->type,18)) {
        Other *other=event->other;
        s32 eligible=(other->field_1e&0x7C) && !func_800A692C(other,10) && !func_800E1CD4(other,15) && other->field_a!=0x5D && !get_flag(p);
        if(eligible) {
            func_80119AE4(p,event->arg,other);
            if(event->type==18) { func_800D3650(p); if(p) p->field08->method0C((char*)p+p->field08->delta08,3); }
            return 1;
        }
    }
    return func_801131F8(p,event);
}
