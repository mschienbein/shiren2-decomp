#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
extern unsigned char D_8013D444,D_8013B780[],D_8013B768[];
extern s32 D_80000300;
extern void func_8006A840(void),func_8006BFD0(s32),func_8006AEB0(s32,void *,void *),func_80034200(u32),func_8006E764(void),func_80060C54(u32),func_800546F4(char *,...),func_8006E6D8(void),func_80058BA8(u16 *,u16 *,u8 *,u8 *),func_80052260(s16),func_80041E20(void),func_8005EF60(void);
extern s32 func_80054DF0(u32);
extern s32 func_8006A8A0(void),func_80058A20(s32);
extern char *func_80048480(u16);
/* Boot forwards its thread argument at 0x80025D98; the game does not use it. */
void func_8006E590(void *argument) {
    unsigned short buttons;
    func_8006A840();
    D_8013D444=1;
    func_8006BFD0(2);
    switch(func_8006A8A0()) {
    case 1: case 2: func_8006AEB0(3,D_8013B780,(void *)0x80000400); break;
    case 0: default: func_8006AEB0(3,D_8013B768,(void *)0x80000400); break;
    }
    func_80034200(0x5A);
    func_8006E764();
    if(D_80000300!=1) {
        func_80060C54(1);
        func_800546F4(func_80048480(0x55B));
        for(;;) func_8006E6D8();
    }
    func_8006E6D8();
    if(func_80058A20(0x10)==0x80) {
        func_80060C54(1);
        func_80054DF0(0xA);
        for(;;) func_8006E6D8();
    }
    func_80058BA8(&buttons,0,0,0);
    if(func_80058A20(0x10)==3 && buttons==0x380C) { func_80052260(0x2B); func_80041E20(); }
    for(;;) { func_8006E6D8(); func_8005EF60(); }
}
