#include "common.h"
void func_80083F9C(unsigned char *text) {
    s32 value;
    if(text) { while((value=*text)!=0) { if((u32)(value-0x56)<0x53) *text=value-0x55; ++text; } }
}
