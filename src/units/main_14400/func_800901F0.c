#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { char pad0[0x14]; s32 field14; s32 field18; char pad1C[8]; u8 field24; char pad25[3]; float field28; float field2C; float field30; float field34; s16 field38; s16 field3A; u8 field3C; u8 field3D; u8 field3E; u8 field3F; u8 field40; } Texture;
typedef struct { char pad0[2]; u8 count; char pad3[5]; Texture **items; } Collection;
extern void *func_80091450(u32);
extern u8 *func_8006A810(u8 *, s32, s32);
extern void func_80091544(void *);
extern void func_8008FEF0(void), func_80090058(void *);
extern s32 func_8008FEF8(void *, void *, void *);
extern void func_8008D3A0(Texture *, s32, void (*)(void), s32 (*)(void *, void *, void *), void (*)(void *));
extern s32 func_8008DF04(void *), func_80090090(void *, Texture *), func_80090144(u16);
extern u16 func_8008DFE4(void *);
extern u32 func_8008E0C4(void *, void *, u32);
extern float func_8008E178(void *);
s32 func_800901F0(void *stream, s32 length, void *storage) {
    u8 unused;
    s32 status = 0;
    Collection *collection = storage;
    s32 count;
    Texture *p;

    /* ODD_C: single-pass error block grouping texture allocation, field parsing and size
     * validation before the shared cleanup; it also shapes the prologue scheduling. */
    do {
        float value;

        p = func_80091450(sizeof(Texture));
        if (!p) {
            status = -1;
            break;
        }
        func_8006A810((u8 *)p, 0, 68);
        func_8008D3A0(p, 0x54585452, func_8008FEF0, func_8008FEF8, func_80090058);
        p->field14 = func_8008DF04(stream);
        func_8008DF04(stream);
        p->field18 = func_8008DF04(stream);
        count = func_80090090(stream, p);
        if (count < 0) {
            status = -1;
            break;
        }
        func_8008E0C4(stream, &p->field24, 1);
        func_8008E0C4(stream, &p->field40, 1);
        /* One reserved byte is read and discarded. */
        func_8008E0C4(stream, &unused, 1);
        p->field38 = func_8008DFE4(stream);
        p->field3E = func_80090144((u16)p->field38);
        p->field3A = func_8008DFE4(stream);
        p->field3F = func_80090144((u16)p->field3A);
        func_8008E0C4(stream, &p->field3C, 1);
        func_8008E0C4(stream, &p->field3D, 1);
        value = func_8008E178(stream);
        count += 21;
        p->field28 = (u32)value;
        value = func_8008E178(stream);
        count += 4;
        p->field2C = (u32)value;
        value = func_8008E178(stream);
        count += 4;
        p->field30 = (u32)value;
        value = func_8008E178(stream);
        count += 4;
        p->field34 = (u32)value;
        count += 4;
        if (count != length) {
            status = -1;
            break;
        }
        {
            u8 index = collection->count++;
            collection->items[index] = p;
        }
    } while (0);
    if (status && p) {
        func_80091544(p);
    }
    return status;
}
