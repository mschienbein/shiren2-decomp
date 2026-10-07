#include "common.h"
typedef unsigned char u8;
extern u8 D_80142F20, D_80147620[];
extern s32 func_800E20CC(void *);
extern u8 func_800C57A0(void *);
extern s32 func_800C5844(void *,u8,u8);
extern s32 func_800E0F40(u8 *);
extern void func_800E43EC(u8 *,u8,u8);
static inline s32 is_active(u8 *arg) { return (arg[0x1E]&0x7C)!=0; }
s32 func_800FF5CC(u8 *arg,u8 *other) {
    s32 useOther=0;
    if(func_800E20CC(arg)) useOther=other && is_active(other);
    if(useOther) func_800E43EC(arg,other[0x1F],other[0x75]);
    else {
        u8 type, amount;
        s32 failed=D_80142F20!=0x4F;
        if(failed) return 0;
        amount=1;
        if(func_800C57A0(D_80147620)&1) { type=(u8)func_800C5844(D_80147620,0x18,0x1D); if(type==0x1D) type=0x17; }
        else { type=arg[0xA]; amount=(u8)func_800E0F40(arg); }
        if(type!=arg[0x1F]) func_800E43EC(arg,type,amount);
    }
    return 1;
}
