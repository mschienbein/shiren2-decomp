#include "common.h"

typedef void (*TaskFn)(void *task);

extern u32 D_8013968C;
extern s32 func_80046240(void);
void *func_80085154(TaskFn handler, s32 value);
void func_800850F8(TaskFn handler, unsigned short value);
void *func_800851B0(s32 id);
void func_800846B8(void *task);
void func_8008865C(void *task);
void func_800853D4(void *task);
void func_80085454(void *task);
void func_8008B678(void *task);
void func_800854D4(void *task);
void func_800869E8(void *task);
void func_800846F0(void *task);
void func_8008692C(void *task);

/* Starts the task or sound for the current action id (0x12A..0x139). */
void func_8004A558(void)
{
    switch (D_8013968C) {
    case 0x12A:
        if ((func_80046240() ^ 1) != 0) {
            func_80085154(func_800846B8, 0);
        }
        break;
    case 0x12B:
        func_800850F8(func_8008865C, 0x14);
        func_800851B0(0x2B);
        break;
    case 0x12C:
        func_800851B0(0x7E);
        break;
    case 0x12D:
        func_80085154(func_800853D4, 0);
        break;
    case 0x137:
        func_80085154(func_80085454, 0);
        break;
    case 0x12E:
        func_800851B0(0xAF);
        break;
    case 0x131:
        func_80085154(func_8008B678, 0);
        break;
    case 0x132:
        func_80085154(func_800854D4, 0);
        break;
    case 0x136:
        func_80085154(func_800869E8, 0);
        break;
    case 0x138:
        func_80085154(func_800846F0, 0);
        break;
    case 0x139:
        func_80085154(func_8008692C, 0);
        break;
    }
}
