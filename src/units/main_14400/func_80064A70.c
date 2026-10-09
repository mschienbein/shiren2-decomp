#include "common.h"

typedef unsigned char u8;

/* 0x34-byte map entry (D_8013C084[]). */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 pad3;
    s32 x4;
    s32 x8;
    u8 padC[0xC];
    u32 data_18; /* PI ROM offset of the section table */
    u8 pad1C[4];
    u8 *packed_20;
    u8 pad24[4];
    u32 rom_28;
    u32 rom_2C;
    u8 pad30[4];
} Entry;

/* 12-byte section table record read from ROM: segmented start and end addresses. */
typedef struct {
    u8 *start;
    u32 field_4;
    u8 *end;
} Section;

/* 0x28-byte pool record. */
typedef struct {
    u8 data[0x28];
} Record;

typedef struct {
    u8 pad0[0xC];
    void *field_0C;
} Obj80064A70;

extern Entry *D_8013B984;
extern Obj80064A70 *D_8013B980;
extern u32 D_8016DB60;
extern u32 D_8016DB64;
extern s32 D_8016DB68;
extern s32 D_8016DB6C;
extern s32 D_8016DB70;
extern u8 *D_8016DB74;
extern s32 D_8016DB78;
extern s32 D_8016DC0C;
extern u8 *D_801DFF7C;
extern u8 *D_801D2550;
extern Record *D_801D40CC;
extern Record **D_801D934C;

s32 func_800627F4(void);
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void *func_80062CCC(void *value, void *base, u32 tag);
s32 func_80083B10(u8 *src, u8 *dst);
void func_800265E0(void *, s32);
void func_80064D5C(Section *section);
void func_80064378(void *data, u32 tag);
s32 func_800627C4(void);
void func_8006A088(u8 arg0, u8 arg1, u8 arg2, u8 arg3);
void func_8006A1C8(s32 x, s32 y);

void *func_80064A70(u32 index)
{
    Section section;
    u32 size;
    void *result = 0;
    void *data;
    u32 used;

    switch (func_800627F4()) {
    case 0:
    default:
        D_8016DB60 = 0x79D0;
        D_8016DB64 = 0x30000;
        D_8016DB68 = 0x15E;
        D_8016DB6C = 0x15E;
        D_8016DB70 = 0x32;
        break;
    case 1:
        D_8016DB60 = 0xC700;
        D_8016DB64 = 0x60000;
        D_8016DB68 = 0x2BC;
        D_8016DB6C = 0x2BC;
        D_8016DB70 = 0x64;
        break;
    }
    func_8006AAF0(&section, D_8013B984->data_18 + 4 + index * 12, 12);
    size = section.end - section.start;
    data = func_80062CCC(section.start, (void *)D_8013B984->data_18, 5);
    D_8016DB74 = 0;
    D_8016DC0C = 0;
    if (size <= D_8016DB60) {
        func_8006AAF0(D_801DFF7C, (u32)data, size);
    } else {
        D_8016DC0C = 1;
    }
    used = func_80083B10(D_8013B984->packed_20, D_801D2550);
    if (used <= D_8016DB64) {
        s32 length;

        used = (used + 7) & ~7;
        length = D_8013B984->rom_2C - D_8013B984->rom_28;
        if (length != 0 && length + used <= D_8016DB64) {
            D_8016DB74 = D_801D2550 + used;
            func_8006AAF0(D_8016DB74, D_8013B984->rom_28, length);
        }
    } else {
        D_8016DC0C = 1;
    }
    D_8016DB78 = 0;
    func_800265E0(D_801D40CC, D_8016DB6C * sizeof(Record));
    {
        Record *record = D_801D40CC;
        Record **slot = D_801D934C;
        Record *end = record + D_8016DB6C;

        for (; record < end; record++) {
            *slot++ = record;
        }
    }
    if (D_8016DC0C == 0) {
        func_80064D5C(&section);
        func_80064378(D_8016DB74, 6);
        if (func_800627C4() == 1) {
            result = D_8013B980->field_0C;
        } else if (D_8016DB60 - size < 0x1DC0) {
            result = 0;
        } else {
            result = D_801DFF7C + size;
        }
    }
    func_8006A088(D_8013B984->r, D_8013B984->g, D_8013B984->b, 0);
    func_8006A1C8(D_8013B984->x4, D_8013B984->x8);
    return result;
}
