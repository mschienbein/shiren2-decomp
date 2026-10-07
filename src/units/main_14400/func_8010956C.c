#include "common.h"

typedef struct { char pad0[0x10]; short offset10; short pad12; s32 (*method14)(void *); char pad18[0xB8]; short offsetD0; short padD2; s32 (*methodD4)(void *,void *); } VTable;
typedef struct Object { char pad0[0x24]; VTable *vtable; char pad28[0x30]; struct Object *field58; char pad5C[0x2C]; s32 field88; char pad8C[0x34]; s32 fieldC0; } Object;
extern s32 func_800E20CC(void *obj),func_800EF210(Object *);
extern void func_800E8FAC(Object *);
s32 func_8010956C(Object *obj) { if(func_800E20CC(obj)) { func_800E8FAC(obj); return 1; } if(!obj->field88) return 0; if(obj->fieldC0) { Object *child=obj->field58; if(child && child->vtable->method14((char *)child+child->vtable->offset10)) obj->field58=0; return obj->vtable->methodD4((char *)obj+obj->vtable->offsetD0,obj->field58); } return func_800EF210(obj); }
