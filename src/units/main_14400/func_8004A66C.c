#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x10]; u16 field_10; } Entry8004A66C;
typedef struct {
    u8 pad0[0x10];
    u16 field_10;
    u8 pad12[2];
    s32 field_14;
    u8 pad18[4];
    s32 field_1C;
    u8 pad20[4];
    s32 field_24;
    s32 field_28;
} Task8004A66C;
extern u32 D_8013968C;
s32 func_80084A10(void);
void func_80061D28(s32 a, s32 b, s32 c, s32 d);
s32 func_80054DF0(u32 mode);
void func_80083724(s32 arg);
void func_800850F8(void (*handler)(void *), u16 value);
void *func_80085154(void (*handler)(void *), s32 value);
Entry8004A66C *func_80084AB4(s32 index);
void func_80084A9C(s32 flag);
void func_80074084(s32 arg);
void func_80083FE0(s32 arg);
s32 func_80048AE4(void);
void func_8008865C(void *task);
void func_80085318(void *record);
void func_800853A4(void *task);
void func_800868D8(void *task);
void func_8004A66C(s32 arg) {
    Task8004A66C *task;

    switch (D_8013968C) {
    case 5:
        if (func_80084A10() == 0) {
            func_80061D28(10, 10, 0x41, 0x2B);
        }
        break;
    case 0xB:
        func_80054DF0(arg);
        break;
    case 0xC:
        func_80083724(arg);
        break;
    case 0x129:
        func_800850F8(func_8008865C, arg);
        break;
    case 0x12F:
        task = func_80085154(func_80085318, 0);
        task->field_14 = task->field_10;
        task->field_10 = func_80084AB4(task->field_10)->field_10;
        task->field_1C = arg;
        break;
    case 0xF:
        func_80084A9C(arg != 0);
        break;
    case 0x10:
        func_80074084(arg);
        break;
    case 0x11:
        func_80083FE0(arg);
        break;
    case 0x130:
        func_80085154(func_800853A4, arg);
        break;
    case 0x133:
    case 0x134:
    case 0x135:
        if (D_8013968C != 0x135) {
            s32 failed = func_80048AE4() ^ 1;
            if (failed) {
                break;
            }
        }
        {
            Task8004A66C *menu = func_80085154(func_800868D8, arg);
            menu->field_24 = D_8013968C == 0x134;
            menu->field_28 = D_8013968C == 0x135;
        }
        break;
    }
}
