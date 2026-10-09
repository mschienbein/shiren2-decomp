#include "common.h"
typedef unsigned char u8;
typedef struct Save800CB618 Save800CB618;
typedef struct Record Record;
/* Stream vtable slot +0x18/+0x1C writes `size` bytes from `data` (void, e.g. func_800CA610). */
typedef struct { u8 pad0[0x18]; short adjust_18; short pad1A; void (*write_1C)(void *, s32, void *); } Methods;
typedef struct { u8 pad0[0x18]; Methods *field_18; } Stream;
extern u8 D_801476D0;
extern u8 D_80147E18[];
extern Stream *D_80147F44;
extern void func_800CB618(Save800CB618 *save, void *buf);
extern void func_800CB884(Record *r, void *buf);
extern s32 func_800CBE18(s32 size);
extern void func_800CA2C0(void *arg0);
void func_800CC034(void *save) {
    s32 size;
    s32 fits;
    if (D_801476D0 < 10) { size = 0x90; func_800CB618(save, D_80147E18); }
    else { size = 0x14; func_800CB884(save, D_80147E18); }
    fits = func_800CBE18(size) == 1;
    if (fits) {
        D_80147F44->field_18->write_1C((u8 *)D_80147F44 + D_80147F44->field_18->adjust_18, size, D_80147E18);
        func_800CA2C0(D_80147F44);
        D_801476D0++;
    }
}
