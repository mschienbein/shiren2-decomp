#include "common.h"
typedef unsigned char u8;

typedef struct { s32 x,y; } Position;
typedef struct { char pad[8]; short offset8; short padA; void (*methodC)(void *,s32); } VTable;
typedef struct { char pad[8]; VTable *vtable; } Entry;
extern s32 func_800CD278(void *),func_800B4888(Position *),func_800CD5C0(void *,Entry *);
extern char *func_800AE674(void *obj);
extern u32 func_800B1C6C(void *pos);
extern void *func_8011422C(void *);
extern void *func_800AC244(u8 id);
extern void func_800ACDD8(void *),func_800498E4(s32,...);
extern void *func_800A6CC0(void *out_position, void *obj);
extern s32 func_80049CB4(s32 id, ...);
static inline s32 isBlocked(Position *position) { s32 blocked=0; if(!(func_800B1C6C(position)&0x2000) || func_800B4888(position)) blocked=1; return blocked; }
void func_80122A80(void *obj,void *value) { Position position; s32 changed; char *id; if(func_800CD278((char *)obj+0xC)<=0) { func_800498E4(0x9B,func_800AE674(obj)); return; } func_800ACDD8(obj); func_800A6CC0(&position,value); if(isBlocked(&position)) { func_800498E4(0xBB); return; } func_80049CB4(0x10A0,value); id=func_800AE674(obj); changed=0; for(;;) { Entry *entry=func_800AC244(0xF0); if(!entry) break; if(func_800CD5C0(func_8011422C(obj),entry)) changed=1; else { entry->vtable->methodC((char *)entry+entry->vtable->offset8,3); break; } } if(changed) func_800498E4(0xB9,id); }
