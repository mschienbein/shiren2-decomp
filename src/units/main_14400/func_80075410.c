#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
/* Unused byte pairs: ROM 0x800745A4/A8 and 0x800745B4/B8 store each byte separately. */
typedef struct {
    s16 field0, field2;
    u8 pad4[2];
    u8 field6, field7;
    u8 pad8[2];
    u16 fieldA;
    char padC[0xC];
    s32 field18;
    char pad1C[0x20];
    u16 field3C;
    u8 field3E, field3F, field40, field41;
    char pad42[4];
    u8 field46, field47;
    char pad48[0x28];
    u8 field70, field71, field72, field73, field74, field75, field76, field77;
    char pad78[0x38];
} Obj;
typedef struct { s16 field0; u8 field2; } Image;
extern Obj D_801DEAB4[], D_801D2C2C[];
extern u8 D_8013D91F;
extern s32 D_8013D8EC, D_8013D8F4;
extern u32 D_8013D890[], D_8013D8B4[], D_8013D8BC[];
typedef struct { s32 cur, repeat; } Repeat;
typedef struct { Repeat menu, shoulder, cbtn; } PadRepeat;
extern PadRepeat D_801A7320[];
typedef signed char s8;
/* Animation scripts consumed by func_80074D88 (same layout as its Anim/AnimFrame). */
typedef struct { s8 id; s8 arg; s16 value; } AnimFrame;
typedef struct { s32 count; AnimFrame *frames; } Anim;
extern Anim D_8014CED8, D_8014CEE0, D_8014CEE8, D_8014CEF0, D_8014CEF8, D_8014CF00, D_8014CF08;
extern s32 func_80074114(void), func_80041C64(s32);
extern Image **func_80074784(s32, s32);
extern void func_80074F34(s32, s32), func_80074D88(Obj *, Anim *, s32);
extern s32 func_8007531C(s32, s32, s32, s32, float, float);
static inline void set_alpha(Obj *p, s32 index, s32 selected, s32 alpha) {
    if (index == 0 || index == func_80041C64(24) || index == func_80041C64(25) || index == func_80041C64(26) || index == func_80041C64(27) || index == func_80041C64(28) || index == selected || (p->fieldA & 0x880) || D_8013D8F4 == 1) p->field46 = alpha;
    else p->field46 = 0;
}
static inline Image **prepare_source(Obj *source, s32 index) { func_80074F34(index, 0); return func_80074784(source->field2, source->field6); }
void func_80075410(void) {
    s32 selected = func_80074114();
    s32 i = 0, alpha;
    s32 inactive = -1;
    for (; i < 30; ++i) {
        Obj *effect = &D_801D2C2C[i];
        Obj *source = &D_801DEAB4[i];
        if (source->field2 != inactive) {
            Image **image, **reference;
            s32 id;
            image = prepare_source(source, i);
            reference = func_80074784(23, D_801DEAB4[0].field6);
            id = D_801A7320[i].cbtn.cur;
            if (id != inactive && D_8013D8BC[id] == 16) {
                source->field70 = 255; source->field71 = 255; source->field72 = 255;
                source->field73 = 0; source->field74 = 0; source->field75 = 0; source->field76 = 0; source->field77 = 0;
                if (D_8013D8EC == 1) {
                    s32 value;
                    alpha = (source->field18 * 8) % 512;
                    value = alpha < 256 ? alpha & 255 : ~alpha & 255;
                    alpha = (u8)(u32)((float)value * 0.8745098f + 31.0f);
                } else alpha = D_8013D91F;
                set_alpha(source, i, selected, alpha);
            }
            if (D_801A7320[i].shoulder.cur != inactive) {
                source->field70 = 0; source->field71 = 0; source->field72 = 0; source->field73 = 0; source->field77 = 0;
                switch (D_8013D8B4[D_801A7320[i].shoulder.cur]) {
                    case 32: source->field74 = 0; source->field75 = 128; source->field76 = 255; break;
                    case 64: source->field74 = 255; source->field75 = 255; source->field76 = 32; break;
                }
                if (D_8013D8EC == 1) {
                    s32 value;
                    alpha = (source->field18 * 8) % 512;
                    value = alpha < 256 ? alpha & 255 : ~alpha & 255;
                    alpha = (u8)(u32)((float)value * 0.8745098f + 31.0f);
                } else alpha = D_8013D91F;
                if (source->fieldA & 16) set_alpha(source, i, selected, alpha);
                else source->field46 = 255;
            }
            if (D_801A7320[i].menu.cur != inactive) {
                float ratio = (float)(*reference)->field2 / (float)(*image)->field2;
                switch ((s32)D_8013D890[D_801A7320[i].menu.cur]) {
                    case 1:
                        if ((effect->field2 != inactive && effect->field3C == 1) || func_8007531C(i, 1, (s32)(ratio * 0.0f), (s32)(ratio * 70.0f), ratio * 1.6f, ratio * 1.6f) != inactive) { effect->field46 = source->field46; effect->field47 = source->field47; func_80074D88(effect, &D_8014CED8, 1); effect->field3F = effect->field3E; }
                        break;
                    case 2:
                        if ((effect->field2 != inactive && effect->field3C == 3) || func_8007531C(i, 3, (s32)(ratio * 40.0f), (s32)(ratio * 40.0f), ratio * 1.6f, ratio * 1.6f) != inactive) { effect->field46 = source->field46; effect->field47 = source->field47; func_80074D88(effect, &D_8014CEE0, 1); effect->field3F = effect->field3E; }
                        break;
                    case 4:
                        if ((effect->field2 != inactive && effect->field3C == 4) || func_8007531C(i, 4, (s32)(ratio * 0.0f), (s32)(ratio * 80.0f), ratio * 1.6f, ratio * 2.2f) != inactive) {
                            u8 opacity; effect->field46 = source->field46; opacity = source->field47; effect->field3E = 0; effect->field3F = 0; effect->field47 = opacity;
                        }
                        break;
                    case 8:
                        if ((effect->field2 != inactive && effect->field3C == 0x135) || func_8007531C(i, 0x135, (s32)(ratio * -4.0f), (s32)(ratio * 86.0f), 2.0f * ratio, 2.0f * ratio) != inactive) { effect->field46 = source->field46; effect->field47 = source->field47; func_80074D88(effect, &D_8014CEE8, 1); effect->field3F = effect->field3E; }
                        break;
                    case 0x80:
                    case 0x800:
                        if ((effect->field2 != inactive && effect->field3C == 0x176) || func_8007531C(i, 0x176, (s32)(ratio * 0.0f), (s32)(ratio * 86.0f), ratio * 3.2f, ratio * 3.2f) != inactive) {
                            effect->field46 = (u8)(u32)(((float)source->field46 / 255.0f) * 179.0f);
                            effect->field47 = source->field47;
                            func_80074D88(effect, &D_8014CEF8, 1);
                            effect->field3F = effect->field3E;
                            if (D_8013D890[D_801A7320[i].menu.cur] == 0x800) effect->field41 = 1; else effect->field41 = 0;
                        }
                        break;
                    case 0x200:
                        if ((effect->field2 != inactive && effect->field3C == 0x18F) || func_8007531C(i, 0x18F, (s32)(ratio * -4.0f), (s32)(ratio * 86.0f), 2.0f * ratio, 2.0f * ratio) != inactive) { effect->field46 = source->field46; effect->field47 = source->field47; func_80074D88(effect, &D_8014CF00, 1); effect->field3F = effect->field3E; }
                        break;
                    case 0x400:
                        if ((effect->field2 != inactive && effect->field3C == 0x190) || func_8007531C(i, 0x190, (s32)(ratio * -2.0f), (s32)(ratio * 85.0f), ratio * 2.7f, ratio * 2.7f) != inactive) { effect->field46 = source->field46; effect->field47 = source->field47; func_80074D88(effect, &D_8014CF08, 1); effect->field3F = effect->field3E; }
                        break;
                    case 0x100:
                        if ((effect->field2 != inactive && effect->field3C == 0x182) || func_8007531C(i, 0x182, (s32)(ratio * -4.0f), (s32)(ratio * 86.0f), 2.0f * ratio, 2.0f * ratio) != inactive) { effect->field46 = source->field46; effect->field47 = source->field47; func_80074D88(effect, &D_8014CEF0, 1); effect->field3F = effect->field3E; }
                        break;
                }
            }
        }
    }
}
