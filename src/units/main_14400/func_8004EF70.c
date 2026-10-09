#include "common.h"

typedef unsigned char u8;

extern u32 D_8013968C;
extern void *D_801476B8;
void func_8008865C(void *);
void func_80086C84(void *);
void func_8008872C(void *);
void func_800850F8(void (*)(void *), unsigned short);
typedef struct {
    u8 pad0[0x20];
    void *arg;
    s32 type;
    s32 kind;
    s32 unk2C;
} Task;
Task *func_80085154(void (*)(void *), s32);
void *func_800851B0(s32);
u8 func_800A8C00(void *);
void func_8004EF70(u8 *info) {
    Task *task;
    s32 kind;

    switch (D_8013968C) {
    case 0x25:
        func_800850F8(func_8008865C, 1);
        if (info[0] == 0xE) {
            func_800851B0(0xC);
        } else {
            func_800851B0(0xB);
        }
        break;
    case 0xB7:
        /* ODD_C: state 0xB7 is handled by the sibling func_8004EEBC; listing it
         * here emits no code (its test folds into the default jump) but it is
         * the sixth case node that makes GCC pivot the compare tree at 0xC6. */
        break;
    case 0xC6:
        func_800851B0(0x18);
        break;
    case 0xC7:
        func_800851B0(0x19);
        break;
    case 0xCA:
        func_80085154(func_80086C84, -1)->arg = info;
        break;
    case 0xD3:
        kind = info[0];
        if (kind >= 5) {
            break;
        }
        if (kind < 3) {
            break;
        }
        if (info[2] & 4) {
            task = func_80085154(func_8008872C, func_800A8C00(D_801476B8));
            task->type = 0x17;
            task->kind = kind;
            task->unk2C = -1;
        }
        break;
    }
}
