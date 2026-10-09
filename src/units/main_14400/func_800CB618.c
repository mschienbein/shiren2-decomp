#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef struct {
    void *field_0;
    u32 field_4;
    u32 field_8;
    s32 field_C;
} BitWriter800CB618;

typedef struct {
    u8 id;
    u8 flags;
} Pair800CB618;

typedef struct {
    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 field_3;
    u8 field_4[4];
    u8 field_8;
    u8 field_9;
    u8 field_A;
    u8 padB;
    s32 field_C;
    s32 field_10;
    u8 field_14;
    u8 field_15;
    u8 field_16;
    u8 field_17[0x50];
    u8 field_67;
    u16 field_68;
    u8 pad6A[2];
    s32 field_6C;
    u8 field_70;
    s8 field_71;
    u8 field_72;
    u8 field_73[0x10];
    u8 field_83;
    s8 field_84;
    u8 field_85;
    u8 field_86[0x10];
    Pair800CB618 pairs[2];
} Save800CB618;

void *func_800A09B0(BitWriter800CB618 *w, void *buf, u32 size);
void func_800A09C4(BitWriter800CB618 *w, void *buf, u32 size);
void func_800A09E8(BitWriter800CB618 *w, u32 value, u32 bits);
void func_800A0A64(BitWriter800CB618 *w, u8 *bytes, s32 count);
s32 func_800CB5D8(u8 value, s32 table);

void func_800CB618(Save800CB618 *save, void *buf) {
    BitWriter800CB618 writer;
    BitWriter800CB618 *w;
    s32 i;

    func_800A09B0(&writer, buf, 0x90);
    w = &writer;
    func_800A09C4(w, writer.field_0, w->field_8);
    func_800A09E8(w, save->field_0, 5);
    func_800A09E8(w, save->field_1, 7);
    func_800A09E8(w, save->field_2, 7);
    func_800A09E8(w, save->field_3, 7);
    func_800A0A64(w, save->field_4, 4);
    func_800A09E8(w, save->field_8, 7);
    func_800A09E8(w, save->field_9, 8);
    func_800A09E8(w, save->field_A, 7);
    func_800A09E8(w, save->field_C, 0x1E);
    func_800A09E8(w, save->field_10, 0x1B);
    func_800A09E8(w, save->field_14, 8);
    func_800A09E8(w, save->field_15, 7);
    func_800A09E8(w, save->field_16, 7);
    func_800A0A64(w, save->field_17, 0x50);
    func_800A09E8(w, save->field_67, 4);
    func_800A09E8(w, save->field_68, 0xA);
    func_800A09E8(w, save->field_6C, 0x18);
    func_800A09E8(w, (u8)func_800CB5D8(save->field_70, 0x32), 6);
    func_800A09E8(w, save->field_71, 8);
    func_800A09E8(w, save->field_72, 5);
    func_800A0A64(w, save->field_73, 0x10);
    func_800A09E8(w, (u8)func_800CB5D8(save->field_83, 0x59), 6);
    func_800A09E8(w, save->field_84, 8);
    func_800A09E8(w, save->field_85, 5);
    func_800A0A64(w, save->field_86, 0x10);
    i = 0;
    for (;;) {
        Pair800CB618 *pair;

        if (i >= 2) {
            break;
        }
        pair = &save->pairs[i];
        func_800A09E8(&writer, (u8)func_800CB5D8(pair->id, 0x7B), 5);
        func_800A09E8(&writer, pair->flags & 1, 1);
        func_800A09E8(&writer, (pair->flags >> 1) & 1, 1);
        i++;
    }
}
