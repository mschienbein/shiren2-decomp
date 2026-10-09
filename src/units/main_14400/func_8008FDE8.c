#include "common.h"
typedef unsigned char u8;
typedef s32 (*Apply)(void *, void *, void *);
typedef struct {
    s32 tag_00, serial_04;
    void (*init_08)(void);
    Apply apply_0C;
    void (*destroy_10)(void *);
    s32 field_14;
    u8 material_18[0x1C];
} Material;
typedef struct { u8 pad_00[2]; u8 count_02; u8 pad_03[5]; Material **materials_08; } Owner;
extern void *func_80091450(u32 size);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
/* Original 8008D3B0/B4/CC stores callback addresses; integer registry parameters are false. */
extern void func_8008D3A0(Material *, s32, void (*)(void), Apply, void (*)(void *));
extern void func_8008FB00(void);
extern s32 func_8008FB08(void *material, void *source, void *destination);
extern void func_8008FBFC(void *);
extern s32 func_8008DF04(void *stream);
extern s32 func_8008FCDC(void *stream, void *material);
extern void func_80091544(void *item);
s32 func_8008FDE8(void *stream, s32 size, Owner *owner) {
    s32 result = 0;
    Material *material;

    /* ODD_C: single-pass error block grouping material allocation and nested-parser size
     * validation before the shared cleanup; it also shapes the prologue scheduling and the
     * original saved-register choice. */
    do {
        material = func_80091450(0x34);
        if (material == 0) {
            result = -1;
            break;
        }
        func_8006A810((u8 *)material, 0, 0x34);
        func_8008D3A0(material, 0x4D415452, func_8008FB00, func_8008FB08, func_8008FBFC);
        material->field_14 = func_8008DF04(stream);
        if (func_8008FCDC(stream, material->material_18) + 4 != size) {
            result = -1;
            break;
        }
        owner->materials_08[owner->count_02++] = material;
    } while (0);
    if (result != 0 && material != 0) {
        func_80091544(material);
    }
    return result;
}
