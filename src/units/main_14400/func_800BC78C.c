#include "common.h"

typedef struct { s32 x,y; } Position;
typedef struct { char pad[0x14]; } Entry;
typedef struct { char pad[0x18]; } Slot;
typedef struct { char pad0[0x3DF]; unsigned char count; char pad3E0[0x57C]; s32 available[16]; } Object;
extern char D_80147620[];
extern Entry D_801431F0[];
extern Slot D_80143330[];
extern unsigned char D_80156ABF;
extern unsigned char func_800C57CC(void *,unsigned char);
extern s32 func_800B68B0(Entry *),func_800A2854(Position *,Position *,Position *),func_800C587C(void *,unsigned char);
extern void *func_800B6A98(void *out,void *room,s32 index);
extern Position *func_800A256C(Position *,Position *,Position *),*func_800A2544(Position *,Position *,Position *);
extern void func_800D1DBC(Slot *,signed char,Entry *),func_800BC69C(Object *,Entry *,s32);
static inline Position *initPosition(Position *p,s32 x,s32 y) { p->x=x; p->y=y; return p; }
s32 func_800BC78C(Object *obj,unsigned char id) {
    Position origin,point,low,deltaLow,high,deltaHigh;
    unsigned char slot=id;
    Entry *entry=0;
    s32 remaining=100;
    for(;;) {
        s32 left=--remaining;
        s32 index,type;
        if(left==-1) break;
        index=func_800C57CC(D_80147620,obj->count-1);
        if(!obj->available[index]) continue;
        entry=&D_801431F0[index];
        type=func_800B68B0(entry);
        if(type>=3) continue;
        if(type==2) {
            Position *lower,*upper;
            func_800B6A98(&origin,entry,0);
            func_800B6A98(&point,entry,1);
            lower=&low;
            func_800A256C(lower,&origin,initPosition(&deltaLow,1,1));
            upper=&high;
            func_800A2544(upper,&origin,initPosition(&deltaHigh,1,1));
            if(func_800A2854(&point,lower,upper)) continue;
        }
        obj->available[index]=0;
        break;
    }
    if(remaining<0) return 0;
    func_800D1DBC(&D_80143330[slot],(signed char)slot,entry);
    func_800BC69C(obj,entry,func_800C587C(D_80147620,D_80156ABF));
    return 1;
}
