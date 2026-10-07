#include "common.h"

typedef unsigned short u16;

/* 116-byte records. */
typedef struct {
    unsigned char pad0[4];
    u16 field_4;
    unsigned char pad6[0xE];
    u32 field_14;
    unsigned char pad18[0x44];
    u32 field_5C;
    u32 field_60;
    unsigned char pad64[4];
    u32 field_68;
    u32 field_6C;
    unsigned char pad70[4];
} Rec80085294;

/* 16-byte by-value argument split across a2/a3 and the caller's stack. */
typedef struct {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
} Quad80085294;

extern Rec80085294 D_801BA380[];

/* func_80084CD4 allocates a task slot, stores the handler at +0x0, returns the slot index. */
s32 func_80084CD4(void (*handler)(Rec80085294 *rec));

Rec80085294 *func_80085294(void (*handler)(Rec80085294 *rec), u32 arg1, Quad80085294 q)
{
    Rec80085294 *rec = &D_801BA380[func_80084CD4(handler)];

    rec->field_14 = arg1;
    rec->field_5C = q.a;
    rec->field_68 = q.b;
    rec->field_60 = q.c;
    rec->field_6C = q.d;
    if (rec->field_4 == 0) {
        rec->field_4 = 1;
    }
    return rec;
}
