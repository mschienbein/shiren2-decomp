#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { char fields[0x18]; } Record;
extern u8 D_8014344C, D_80143448, D_80143391;
extern u16 D_8014344A;
extern Pos D_80143388, D_80143360;
extern s32 D_80143444;
extern Record D_80143330[2];
extern unsigned char D_80143434[];
void func_800D1D90(Record *record);
void func_800D458C(void *list);
void func_800B509C(void);
void func_800B512C(s32 notify);
static inline void clear_x(Pos *position) { position->x = 0; }
static inline void clear_y(Pos *position) { position->y = 0; }
void func_800B1080(void) {
    s32 i = 0;
    Record *p = D_80143330;
    D_8014344C = 0;
    D_8014344A = 0;
    clear_x(&D_80143388);
    clear_y(&D_80143388);
    clear_x(&D_80143360);
    clear_y(&D_80143360);
    do { func_800D1D90(p); ++i; ++p; } while (i < 2);
    D_80143448 = 0;
    D_80143444 = 0;
    func_800D458C(D_80143434);
    D_80143391 = 0;
    func_800B509C();
    func_800B512C(0);
}
