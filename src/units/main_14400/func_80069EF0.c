#include "common.h"

typedef unsigned char u8;

/* libultra Lights3 layout: an 8-byte ambient light followed by three 16-byte directional lights. */
typedef struct { u8 col[3]; u8 pad1; u8 colc[3]; u8 pad2; } Ambient;
typedef struct { u32 words[4]; } Light;
typedef struct { Ambient a; Light l[3]; } Lights3;

/* Two consecutive Lights3 records at 0x8013C990 and 0x8013C9C8 (0x70 bytes). */
extern Lights3 D_8013C990[];

/* Set the first light record's ambient color (col and its copy colc). */
void func_80069EF0(s32 r, s32 g, s32 b) {
    Ambient *ambient = &D_8013C990[0].a;

    ambient->col[0] = ambient->colc[0] = r;
    ambient->col[1] = ambient->colc[1] = g;
    ambient->col[2] = ambient->colc[2] = b;
}
