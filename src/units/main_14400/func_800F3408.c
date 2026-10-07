#include "common.h"
typedef unsigned char u8;
typedef struct { void *field_0; s32 field_4; } Pair;
typedef struct { u8 field_0[0x10]; Pair *field_10; } Context;
typedef struct { u8 field_0[0x7C]; unsigned short field_7C; } Object;
extern short func_800F0D90(Object *,Pair *);
extern void func_800E3884(Object *,Pair *,s32);
extern unsigned short func_800E08B0(Object *);
extern s32 func_800E1CD4(Object *,s32),func_800A65B8(Object *,void *),func_80049CB4(s32,...);
extern char *func_800A3B20(Object *);
extern void func_800497F0(s32,...),func_800A7B18(void *,Object *,s32,s32);
void func_800F3408(Object *arg,Context *ctx) {
    Pair *pair=ctx->field_10;
    short amount=func_800F0D90(arg,pair);
    if(amount) {
        s32 enabled=0;
        func_800E3884(arg,pair,amount);
        if((arg->field_7C>>8)&1) { if(amount>0 && func_800E08B0(arg)) enabled=!func_800E1CD4(arg,0xF); }
        if(enabled) {
            void *other=pair->field_0;
            if(other && func_800A65B8(arg,other)==1 && pair->field_4==1) {
                s32 text=func_80049CB4(0x63,other);
                func_800497F0(0x4F,text,func_800A3B20(arg));
                amount=amount>>2;
                if(!amount) amount=1;
                func_800A7B18(other,arg,amount,4);
            }
        }
    }
}
