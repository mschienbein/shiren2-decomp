#include "common.h"
typedef struct { char pad[0x60]; short field_60; void (*field_64)(void*); } Methods;
typedef struct { char pad[0x24]; Methods *field_24; } Obj;
extern u32 D_8013960C;
extern s32 func_800EAF38(Obj *, s32);
extern s32 func_800EB350(Obj*);
extern void func_800EBF34(Obj*),func_800EBCD0(Obj*);
void func_800EDF78(Obj *p, s32 notify) { func_800EAF38(p, notify); D_8013960C<<=1; func_800EB350(p); func_800EBF34(p); func_800EBCD0(p); D_8013960C>>=1; p->field_24->field_64((char*)p+p->field_24->field_60); }
