#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_0[4]; void *field_4; const char *field_8; u32 field_C; } Offer;
typedef struct { u8 pad_0[0x84]; u32 field_84; } Actor;
typedef struct Buf800D8FB0 Buf800D8FB0;
extern Actor *D_801476B8;
extern u8 D_80140160[];
extern void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_80048480(u16 id);
extern s32 func_800327C0(char *dst, const char *fmt, ...);
extern s32 func_80094CE8(void *unused, char *text);
extern Buf800D8FB0 *func_800D8FB0(u32 size);
extern Buf800D8FB0 *func_800D9AD0(Buf800D8FB0 *obj, void *target);
Buf800D8FB0 *func_800F6A00(Offer *offer) {
    char text[64];
    Buf800D8FB0 *result = 0;
    func_800498E4(0x1CA, offer->field_8, offer->field_C);
    func_80049BF0(0);
    func_80049CB4(2);
    func_800327C0(text, func_80048480(0x1CB), offer->field_8);
    if ((u8)func_80094CE8(D_80140160, text) == 1) {
        if (D_801476B8->field_84 < offer->field_C) {
            func_800498E4(0x1CE, offer->field_8);
        } else {
            func_800498E4(0x1CC, offer->field_8);
            result = func_800D9AD0(func_800D8FB0(12), offer->field_4);
        }
    } else {
        func_800498E4(0x1CD, offer->field_8);
    }
    func_80049BF0(0);
    func_80049CB4(2);
    return result;
}
