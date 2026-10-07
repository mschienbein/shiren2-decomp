#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;

/* func_800612D4 fills both words of each pair with func_8006A8D8 allocation results. */
typedef struct {
    void *a;
    void *b;
} BufferPair;

extern BufferPair D_8016DB38;
extern BufferPair D_8016DB40;
extern BufferPair D_8016DB48;
extern s32 D_8016DB18; /* numeric mode, compared with 1 and 3 */
/* Allocation results and fixed offsets into them stored by func_800612D4. */
extern f32 *D_8013B804; /* float grid indexed by func_80062430/func_80062554 */
extern void *D_8013B80C;
extern void *D_801DFF7C;
extern void *D_801D2550;
extern void *D_801DE97C;
extern void *D_801D9350;
extern void *D_801D40CC;
extern void *D_801D2564;
extern void *D_801D934C;
extern void *D_801D85A8;
extern void *D_801D2C08;
extern void *D_801D2558;
extern void *D_801D40D4;
extern void *D_801E02A4;
extern void *D_801D2560;
extern void *D_801D2C00;
extern void *D_801D2554;
extern void *D_8013C760;
extern u8 *D_8013C764;

void func_80061650(void) {
    D_8016DB38.a = 0;
    D_8016DB38.b = 0;
    D_8016DB40.a = 0;
    D_8016DB40.b = 0;
    D_8016DB18 = 0;
    D_8013B804 = 0;
    D_8013B80C = 0;
    D_801DFF7C = 0;
    D_801D2550 = 0;
    D_801DE97C = 0;
    D_801D9350 = 0;
    D_801D40CC = 0;
    D_801D2564 = 0;
    D_801D934C = 0;
    D_801D85A8 = 0;
    D_801D2C08 = 0;
    D_801D2558 = 0;
    D_801D40D4 = 0;
    D_801E02A4 = 0;
    D_801D2560 = 0;
    D_801D2C00 = 0;
    D_801D2554 = 0;
    D_8013C760 = 0;
    D_8013C764 = 0;
    D_8016DB48.a = 0;
    D_8016DB48.b = 0;
}
