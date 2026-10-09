#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;
typedef struct {
    void (*unk0)(void *); u16 unk4, unk6, unk8, unkA, unkC, unkE, unk10, unk12;
    s32 unk14, unk18, unk1C, unk20, unk24, unk28, unk2C;
    f32 unk30, unk34, unk38, unk3C, unk40, unk44, unk48, unk4C, unk50, unk54, unk58;
    s32 unk5C, unk60, unk64, unk68, unk6C, unk70;
} Task;
typedef struct { char text[64]; } Line;
extern s32 D_8013E924;
extern s32 D_8013E918, D_8013E91C;
extern s32 D_801C3390;
extern Task D_801BA380[];
extern Line D_801BF2D0[];
extern u8 D_801C3290[];
extern void func_80084728(void *);
extern s32 func_80054220(void), func_8005485C(void), func_80054920(void), func_8006D550(s32);
extern void func_8005422C(s32), func_800836D4(s32);
extern void func_800546BC(char *, void *);
extern void func_800265E0(void *, s32);
void func_800843D8(Task *task) {
    s32 count, i;
    u32 delay;
    switch (task->unk8) {
    case 0:
        count = 0;
        if (func_80054220() != 0) return;
        for (i = 0; i < task->unk6; i++) {
            if (D_801BA380[i].unk0 == task->unk0 || task->unk0 == func_80084728) count++;
        }
        if (count) return;
        task->unk1C = 0;
        if (func_8005485C() != 0) {
            if ((u32)D_8013E918 < (u32)D_8013E91C) task->unk1C = D_8013E91C - D_8013E918;
        } else if (!D_801C3390 && (u32)D_8013E918 < (u32)D_8013E91C) task->unk1C = D_8013E91C;
        D_801C3390 = 0;
        task->unk28 = 0;
        task->unk8++;
        /* fall through */
    case 1:
        if (func_80054920() != 0) return;
        delay = task->unk1C;
        if (delay) { task->unk1C = delay - 1; return; }
        i = task->unk28;
        func_800546BC(D_801BF2D0[task->unk14 + i].text, 0);
        D_8013E918 = 0;
        if (task->unk14 + i < 255) func_800265E0(&D_801BF2D0[task->unk14 + i], 64);
        task->unk1C = D_8013E91C;
        task->unk28++;
        if (task->unk28 >= task->unk24) {
            func_800836D4(0);
            task->unk8++;
        }
        if (D_801C3290[task->unk14 + i]) {
            func_8005422C(1);
            if (task->unk12 & 0x1000) { task->unk1C = 20; task->unk8 = 4; }
            else task->unk8 = 3;
        }
        break;
    case 2:
        if (!task->unk2C) { D_8013E924 = 1; task->unk4 = 4; }
        else task->unk2C--;
        break;
    case 3:
        i = func_8006D550(6);
        if (i == 1) {
            func_8005422C(0);
            task->unk1C = 0;
            if (task->unk28 >= task->unk24) { task->unk2C = 0; task->unk8 = 2; }
            else task->unk8 = 1;
        }
        break;
    case 4:
        if (task->unk1C-- == 0) {
            func_8005422C(0);
            task->unk1C = 0;
            if (task->unk28 >= task->unk24) { task->unk2C = 0; task->unk8 = 2; }
            else task->unk8 = 1;
        }
        break;
    }
}
