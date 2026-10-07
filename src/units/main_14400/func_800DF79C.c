#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x8]; s32 state; } Obj800DF79C;
void func_800C9778(void);
void func_800498E4(s32 id, ...);
void func_800458BC(s32 arg);
void func_80045AAC(void);
void func_80045BA4(void);
void func_80045C84(s32 arg);
void func_80048380(void);
void func_80049BF0(s32 arg);
s32 func_80049CB4(s32 id, ...);
s32 func_8006D44C(s32 arg0, s32 arg1);
s32 func_800DF79C(Obj800DF79C *obj) {
    switch (obj->state) {
    case 3:
        func_800498E4(0x21F);
        func_80049CB4(2);
        func_8006D44C(0, 0x6ACFC0);
        /* fallthrough */
    case 2:
    case 4:
        func_80045C84(8);
        break;
    case 1:
    default:
        func_800C9778();
        func_800498E4(0x21D);
        func_800458BC(0x48);
        func_80045AAC();
        func_80049CB4(0x129, 0xF);
        func_80049BF0(0);
        func_80049CB4(2);
        func_800498E4(0x21E);
        func_80049CB4(0x129, 0xF);
        func_80049CB4(2);
        func_80045BA4();
        func_8006D44C(0, 0x6ACFC0);
        break;
    }
    func_80048380();
    return 4;
}
