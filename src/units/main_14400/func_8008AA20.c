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
typedef union { s32 word; struct { u16 hi, lo; } half; } Pos;
extern s32 func_800748F8(s32, s32, s32, s32, s32, s32);
extern s32 func_800751B4(s32, s32);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern Sprite *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
typedef struct { f32 values[8]; } Camera;
extern void func_80059728(Camera *), func_8005ABBC(Camera *, s32, s32);
extern void func_80061820(s32, s32), func_80061908(Pos, Pos);
extern s32 func_80042A08(s32), func_80042A2C(s32);
extern s32 func_800421D4(s32, s32);
extern void func_80062658(s32, s32), func_800740E4(s32), func_8007D3A8(s32), func_8007D8A0(s32);
void func_8008AA20(Task *task) {
    Camera camera;
    Pos px, py;
    switch (task->unk8) {
    case 0:
        func_80059728(&camera);
        camera.values[0] = (task->unk5C << 5) + 16;
        camera.values[2] = (task->unk68 << 5) + 16;
        camera.values[3] = 6.08f;
        camera.values[6] = 262.0f;
        func_80061820(task->unk5C, task->unk68);
        px.word = task->unk5C; py.word = task->unk68;
        func_80061908(px, py);
        func_8005ABBC(&camera, 0, 0);
        task->unk1C = 70; task->unk8++;
        break;
    case 1:
        func_80059728(&camera);
        camera.values[3] = 6.063f; camera.values[4] = 0.786f; camera.values[6] = 350.0f;
        func_8005ABBC(&camera, 68, 0);
        func_800740E4(0); func_8007D3A8(0); func_8007D8A0(1);
        task->unkE = 1; task->unk8++;
        /* fall through */
    case 2:
        if (task->unk1C-- == 0) {
            func_80059728(&camera);
            camera.values[4] = 0.0f; camera.values[3] = 6.045f; camera.values[6] = 229.0f;
            func_8005ABBC(&camera, 0, 0);
            task->unk1C = 2; task->unk8++;
        }
        break;
    case 3:
        if (task->unk1C-- == 0) task->unk8++;
        break;
    case 4: {
        s32 i;
        s32 target = task->unk24;
        for (i = 0; i < 30; i++) {
            Sprite *sprite = func_8007946C(0, i);
            if (sprite && sprite->unk2 != -1) {
                if (i == target && task->unk12 & 0x4000) {
                    func_80076044(i, 0x145, 2, 8, 0);
                    func_800751B4(task->unk14, 0x8000);
                } else if (!func_80042A08(i) && !func_80042A2C(i)) {
                    if ((task->unk68 << 7) + 64 < sprite->unk10) {
                        func_80079560(0, i, 1);
                    } else {
                        sprite->unk14 = 0.0f;
                        func_80076044(i, sprite->unk3C, 2, 0, 1);
                    }
                }
            }
        }
        for (i = 0; i < 110; i++) {
            Sprite *object = func_8007946C(3, i);
            if (object) {
                u32 kind = (u16)object->unk2;
                if (object->unk2 != -1) {
                    s32 x = object->unkC >> 7;
                    s32 y = object->unk10 >> 7;
                    if ((u32)(kind - 0xD0) >= 25 && (u32)(kind - 0xEF) >= 6 && !func_800421D4(x, y)) {
                        func_80062658(x, y);
                    }
                }
            }
        }
        task->unk1C = 200;
        task->unk8++;
        break;
    }
    case 5:
        if (task->unk1C-- != 0) {
            s32 i;
            for (i = 0; i < 30; i++) {
                Sprite *sprite = func_8007946C(0, i);
                if (sprite && sprite->unk2 != -1 && !func_80042A08(i) && !func_80042A2C(i)) {
                    sprite->unk14 += (i == task->unk24) ? 0.4f : (i % 4) * 0.1f + 0.4f;
                }
            }
        } else {
            task->unk1C = 34;
            task->unk8++;
        }
        break;
    case 6:
        if (task->unk1C-- == 0) {
            func_800740E4(1); func_8007D3A8(1); func_8007D8A0(0); task->unk4 = 4;
        }
        break;
    }
}
