#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_0[0x90]; short field_90,field_92; s32 (*field_94)(void *,s32,s32,u8,s32); } VTable;
typedef struct { u8 field_0[0x24]; VTable *field_24; u8 field_28[0xD]; u8 field_35[10]; u8 field_3F[3]; u8 field_42,field_43,field_44; } Object;
extern s32 func_800E1DA0(Object *);
extern u32 func_800E10D0(const Object *), func_800E1148(const Object *);
static inline void expire(Object *arg,s32 index) { arg->field_24->field_94((char *)arg+arg->field_24->field_90,1,index,0,0); }
void func_800E5020(Object *arg) {
    s32 i;
    for(i=0;i<10;++i) {
        u8 value=arg->field_35[i];
        if(value && value!=0xFF) { if(value==1) expire(arg,i); else arg->field_35[i]=value-1; }
    }
    if(arg->field_42 && arg->field_42!=0xFF) {
        s32 failed=func_800E1DA0(arg)!=1;
        if(failed) { if(arg->field_42==1) expire(arg,func_800E10D0(arg)); else --arg->field_42; }
    }
    if(arg->field_44 && arg->field_44!=0xFF) {
        s32 index=func_800E1148(arg);
        if(arg->field_44==1) expire(arg,index); else --arg->field_44;
    }
    { u8 value=arg->field_43;
      if(value && value!=0xFF) { if(value==1) expire(arg,0x11); else arg->field_43=value-1; }
    }
}
