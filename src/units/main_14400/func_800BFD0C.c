#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad : 3; u8 bit4 : 1; u8 bit3 : 1; u8 bit2 : 1; u8 low : 2; } Flags800BFD0C;
typedef struct { s32 x; s32 y; } Pos800BFD0C;
typedef struct { Pos800BFD0C start; Pos800BFD0C end; } Area800BFD0C;
typedef struct { Area800BFD0C area; s32 extra; } AreaEntry800BFD0C;
typedef struct { u8 pad0[0x3FC]; u8 side; u8 pad3FD[3]; s8 mode; } Ctx800BFD0C;
extern Flags800BFD0C D_80142F1B;
extern u8 D_80142F00;
extern AreaEntry800BFD0C D_801431F0[];
extern Area800BFD0C D_801429C0;
void *func_800AAC20(s32 kind);
void *func_800AB9C4(void);
s32 func_800AE2A4(void *obj, s32 allowItem, s32 allowWater, void *area);

static inline void copy_pos(Pos800BFD0C *dst, Pos800BFD0C *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline void copy_area(Area800BFD0C *dst, Area800BFD0C *src) {
    copy_pos(&dst->start, &src->start);
    copy_pos(&dst->end, &src->end);
}

void func_800BFD0C(Ctx800BFD0C *ctx) {
    s32 i;
    void *last;
    Area800BFD0C area;
    u32 bit2 = D_80142F1B.bit2;
    if (bit2 != 0) {
        return;
    }
    i = D_80142F00;
    for (;;) {
        void *obj;
        if (--i == -1) {
            break;
        }
        obj = func_800AAC20(0);
        if (obj == 0) {
            continue;
        }
        if (ctx->mode == 11) {
            copy_area(&area, &D_801431F0[ctx->side ^ 1].area);
            func_800AE2A4(obj, 0, 0, &area);
        } else {
            func_800AE2A4(obj, 0, 0, &D_801429C0);
        }
    }
    last = func_800AB9C4();
    if (last != 0) {
        func_800AE2A4(last, 0, 0, &D_801429C0);
    }
}
