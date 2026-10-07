#include "common.h"
typedef unsigned char u8;
/* PI ROM address ranges, not CPU pointers. */
typedef struct { u32 gfxStart; u32 gfxEnd; u32 dataStart; u32 dataEnd; } MapEntry;
typedef float f32;
extern f32 *D_8013B804; /* float grid pointer (func_80064A70 result); cleared here */
extern s32 D_8016FD40;
extern MapEntry D_8013C800[];
extern MapEntry *D_8013C768;
extern void *D_8013C760;
extern u8 *D_8013C764;
u8 *func_8006B580(s32 a);
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_80135ED4(s32 a);
void func_8013591C(void *src, void *dst, s32 width, void *work);
void func_8005EFF0(void *dst, void *src, u32 size);
void func_8006B4FC(void *p);
void func_8006B6F4(void *p);
void func_8006A088(u8 a, u8 b, u8 c, u8 d);
void func_8006A1C8(s32 a, s32 b);
void func_80069A68(u32 *index) {
    u8 *work;
    u8 *data;
    u32 size;
    u32 i;
    u32 bit;
    s32 out;
    u32 n;
    D_8013B804 = 0;
    if (*index >= 25) *index = 0;
    n = *index;
    D_8016FD40 = n;
    D_8013C768 = &D_8013C800[n];
    work = func_8006B580(1);
    size = 0x2580;
    data = work + 0x3840;
    func_8006AAF0(data, D_8013C768->gfxStart, D_8013C768->gfxEnd - D_8013C768->gfxStart);
    func_80135ED4(0xFF);
    func_8013591C(data, D_8013C760, 0x140, work);
    data = work + size;
    func_8006AAF0(data, D_8013C768->dataStart, D_8013C768->dataEnd - D_8013C768->dataStart);
    func_8005EFF0(work, data + *(s32 *)(data + 4), size);
    for (out = i = 0; i < size; i++) {
        for (bit = 0; bit < 8; bit += 2) {
            D_8013C764[out++] = (((work[i] >> (7 - bit)) & 1) << 4) | ((work[i] >> (6 - bit)) & 1);
        }
    }
    func_8006B4FC(work);
    func_8006B6F4(work);
    func_8006A088(0, 0, 0, 0);
    func_8006A1C8(0x3C5, 0x3CC);
}
