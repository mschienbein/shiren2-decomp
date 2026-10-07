#include "common.h"
typedef unsigned char u8;

typedef struct { s32 x,y; } Position;
typedef struct { char pad0[0x10]; short offset10; short pad12; s32 (*method14)(void *); char pad18[0x30]; short offset48; short pad4A; s32 (*method4C)(void *,Position *); } VTable;
typedef struct { Position position; char pad8[0x16]; unsigned char flags1E; char pad1F[5]; VTable *vtable; } Object;
extern s32 func_800E1CC4(Object *,s32),func_800A455C(Object *,Object *,s32),func_800B20B4(Object *,Position *);
static inline void copyPosition(Position *dest, Position *src) { dest->x=src->x; dest->y=src->y; }
u8 func_800A6420(Object *obj,Object *target) {
    Position position;
    s32 blocked;
    if(!target) return 3;
    copyPosition(&position,&target->position);
    blocked=0;
    if (!obj->vtable->method4C((char *)obj+obj->vtable->offset48,&position) || ((target->flags1E & 0x7C) && func_800E1CC4(target,1)) || target->vtable->method14((char *)target+target->vtable->offset10)) blocked=1;
    if(blocked) return 3;
    if(func_800A455C(obj,target,1)) return 0;
    if(func_800B20B4(obj,&position)) return 1;
    return 2;
}
