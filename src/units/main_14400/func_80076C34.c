#include "common.h"
typedef short s16;
typedef unsigned char u8;
typedef float f32;
typedef struct { s16 x, y, kind; } Point;
/* +0 is a signed count byte (ROM 0x80076618: lb), followed by padding; unused here. */
typedef struct { u8 pad0[2]; Point unk2[3]; Point unk14[3]; s16 unk26[4]; } Layout;
typedef struct { s16 unk0, unk2; u8 unk4[2], unk6; char unk7[0x19]; f32 unk20; char unk24[0x8C]; } State;
typedef struct { s32 unk0; u8 unk4; } Sprite;
extern Layout D_801A79E8[];
extern State D_801DEAB4[];
extern s32 func_800627C4(void);
extern s32 func_80042734(s32);
extern void func_8004194C(s32, s32 *, s32 *);
extern Sprite **func_80074784(s32, s32);
extern s32 func_800625FC(s32, s32);
extern s32 func_80062554(s32, s32);
s32 func_80076C34(s32 arg0, s32 arg1, s32 arg2) {
    s32 x = 0, y = 0;
    s32 kind = 0;
    s32 result = -1;
    s32 height;
    Layout *layout;
    State *state;
    switch ((u32)func_800627C4()) {
    case 2: height = -4; break;
    case 1: height = -15; break;
    case 3:
    default: height = -8; break;
    }
    layout = &D_801A79E8[arg0];
    state = &D_801DEAB4[arg0];
    switch (arg1) {
    case 0:
        kind = layout->unk14[arg2].kind;
        x = layout->unk14[arg2].x;
        y = layout->unk14[arg2].y;
        break;
    case 2:
        kind = func_80042734(arg0);
        func_8004194C(arg0, &x, &y);
        break;
    case 1:
        kind = layout->unk2[arg2].kind;
        x = layout->unk2[arg2].x;
        y = layout->unk2[arg2].y;
        break;
    }
    switch (kind) {
    /* ODD_C: kind 0 (no point) leaves result at -1; the label shapes codegen: without it 52 words differ (564 vs 576 bytes). */
    case 0: break;
    case 1:
        if (state->unk2 == 0x49) {
            result = (height - (s32)((*func_80074784(0x49, state->unk6))->unk4 * state->unk20)) * 4;
        } else if (func_800625FC(x, y) & 0x80) result = -128;
        else result = func_80062554(x, y) * 4;
        break;
    case 2:
        if (func_800625FC(x, y) & 0x2080) result = 0;
        else result = func_80062554(x, y) * 4;
        break;
    }
    return result;
}
