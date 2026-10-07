#include "common.h"

typedef struct { char pad[0x38]; short offset38; short pad3A; void *(*method3C)(void *,u32); } VTable;
typedef struct { s32 field0; VTable *vtable; } Child;
typedef struct { Child *child; void *field4; } Object;
typedef struct { s32 fields[4]; } Selection;
typedef struct { s32 fields[2]; } Request;
/* The word at +0x2CC belongs to the same context as callback and field408. */
typedef struct { char pad0[0x2CC]; s32 field2CC; char pad2D0[0x1C]; s32 (*callback)(void *); char pad2F0[0x118]; unsigned short field408; } Context;
extern Context D_801404E0;
extern char D_8014AB68[];
extern s32 func_800A1730(void *);
extern s32 func_800CE46C(Child *,s32 (*)(void *)),func_800957C0(Context *,Selection *,s32,void *,s32),func_8009A0EC(Context *);
extern void func_80097B90(Context *,Child *,void *,s32,void *,s32);
extern void *func_800D0190(Request *output,Child *helper,void *item);
extern void *func_800D8FB0(u32 size),*func_800DD590(void *,Request *);
static inline void setContextField2CC(Context *context,s32 value) { context->field2CC=value; }
s32 func_800A18AC(Object *obj) { Selection selection; Request request; s32 failed=func_800CE46C(obj->child,func_800A1730)!=1; if(failed) return 0; func_80097B90(&D_801404E0,obj->child,0,0x1000,D_8014AB68,0); D_801404E0.field408=0x1F0; setContextField2CC(&D_801404E0,0); D_801404E0.callback=func_800A1730; failed=func_800957C0(&D_801404E0,&selection,1,0,0)!=1; if(failed) return 1; if(func_8009A0EC(&D_801404E0)&1) return 2; { Child *child=obj->child; void *value=child->vtable->method3C((char *)child+child->vtable->offset38,selection.fields[0]); func_800D0190(&request,obj->child,value); } obj->field4=func_800DD590(func_800D8FB0(0x10),&request); return 3; }
