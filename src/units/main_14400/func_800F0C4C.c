#include "common.h"
extern s32 func_800A7024(unsigned char *);
extern s32 func_800E0F40(unsigned char *);
extern void *func_800A83D0(s32,unsigned char);
extern s32 func_800E4604(unsigned char *,void *);
s32 func_800F0C4C(unsigned char *arg) {
    s32 value;
    void *found;
    if(func_800A7024(arg)) return 0;
    value=arg[0xA];
    found=func_800A83D0(value,(unsigned char)func_800E0F40(arg));
    return !found ? 0 : func_800E4604(arg,found);
}
