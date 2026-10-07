#include "common.h"
typedef unsigned char u8;

typedef struct { s32 x,y; } Position;
typedef struct Object { Position position; char pad8[0x50]; struct Object *target; char pad5C[8]; Position destination; } Object;
typedef struct { s32 fields[2]; } Action;
extern s32 func_800A692C(Object *,s32),func_800F3310(Object *),func_800F17A8(Object *,Position *,s32),func_800F1024(Object *),func_800E1CD4(Object *,s32),func_800E8350(Object *),func_800E7104(Object *),func_800E66EC(Object *);
extern u32 func_800B1C6C(void *pos);
extern unsigned char func_800A6420(Object *,Object *);
extern void func_800A665C(Object *,Action *),func_800F06E4(Object *);
extern void *func_800A6538(void *out_direction,void *obj,void *target);
extern void *func_800B36C4(void *out,void *pos,u8 kind,s32 arg3);
static inline void copyPosition(Position *dest,Position *src) { dest->x=src->x; dest->y=src->y; }
s32 func_80102484(Object *obj) { Position start,destination,next; Action action; Object *target; s32 active,failed; if(func_800A692C(obj,0x12)) return func_800F3310(obj); target=obj->target; copyPosition(&start,&obj->position); switch(func_800A6420(obj,target)) { case 1: case 2: if(func_800F17A8(obj,&start,0)) { func_800A6538(&action,obj,target); func_800A665C(obj,&action); active=0; if(!(func_800B1C6C(target)&0x4000)) active=func_800F1024(obj)!=0; if(active) func_800F06E4(obj); return 0; } break; case 0: func_800F06E4(obj); return 0; default: if(func_800F17A8(obj,&start,0)) return 0; break; } if(func_800E1CD4(obj,0x10)) return func_800E8350(obj); destination.x=obj->destination.x; destination.y=obj->destination.y; failed=func_800F17A8(obj,&destination,0)!=1; if(failed) { func_800B36C4(&next,&start,0,0); if(!(next.y|next.x)) return func_800E7104(obj); obj->destination=next; } return func_800E66EC(obj); }
