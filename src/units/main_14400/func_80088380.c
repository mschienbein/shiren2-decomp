#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;
typedef double f64;
typedef struct {
    void (*unk0)(void *); u16 unk4, unk6, unk8, unkA, unkC, unkE, unk10, unk12;
    s32 unk14, unk18, unk1C, unk20, unk24, unk28, unk2C;
    f32 unk30, unk34, unk38, unk3C, unk40, unk44, unk48, unk4C, unk50, unk54, unk58;
    s32 unk5C, unk60, unk64, unk68, unk6C, unk70;
} Task;
typedef struct {
    s16 unk0, unk2; u8 pad4[2], unk6, unk7, unk8, unk9; u16 unkA;
    s16 unkC, unkE, unk10; u8 unk12, unk13; f32 unk14;
    u8 pad18[4]; f32 unk1C, unk20, unk24; u8 pad28[0x10]; f32 unk38;
    u16 unk3C; u8 unk3E, unk3F, unk40; u8 pad41[5]; u8 unk46, unk47;
    u8 pad48[0x28]; u8 unk70, unk71, unk72, unk73, unk74, unk75, unk76, unk77;
} Sprite;
extern s32 func_800748F8(s32, s32, s32, s32, s32, s32);
extern s32 func_800751B4(s32, s32);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern Sprite *func_8007946C(s32, s32);
extern u8 D_801C3395[];
void func_80088380(Task *task) {
    Sprite *sprite;
    f32 alpha;
    u16 state = task->unk8;

    switch (state) {
    case 0: {
        s32 slot = task->unk14;
        if (D_801C3395[slot] == 0) {
            D_801C3395[slot] = 1;
            func_800748F8(task->unk14, task->unk24, task->unk5C, task->unk68, task->unk2C, task->unk28);
        }
        sprite = func_8007946C(0, task->unk14);
        func_80076044(task->unk14, sprite->unk3C, 2, 8, 3);
        func_800751B4(task->unk14, 0x8000);
        {
            f32 height = sprite->unk14 - 101.0f;
            sprite->unk73 = 0xFF;
            sprite->unk77 = 0xFF;
            sprite->unk70 = 0;
            sprite->unk71 = 0;
            sprite->unk72 = 0;
            sprite->unk74 = 0;
            sprite->unk75 = 0;
            sprite->unk76 = 0;
            sprite->unk14 = height;
        }
        task->unk30 = 21.0f;
        task->unk34 = 1.0f;
        task->unk1C = 15;
        task->unkE = 1;
        task->unk8++;
        return;
    }
    case 1:
        sprite = func_8007946C(0, task->unk14);
        if (task->unk1C-- != 0) {
            alpha = (f32)sprite->unk73 - task->unk34;
            sprite->unk73 = (u32)alpha;
            sprite->unk14 = sprite->unk14 + task->unk30;
            task->unk30 = task->unk30 * 0.8;
            task->unk34 = task->unk34 * 1.31;
            return;
        }
        func_80076044(task->unk14, sprite->unk3C, 1, 8, 3);
        func_800751B4(task->unk14, 0);
        task->unk4 = 4;
        return;
    }
}
