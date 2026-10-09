#include "common.h"

typedef unsigned char u8;

/* Four-byte item descriptor copied as one word; byte 0 is the kind (0 = none). */
typedef struct {
    u32 kind : 8;
    u32 data : 24;
} ItemWord;

/* Received link packet (D_80147EA8, a 0x9C-byte buffer filled by func_800CBF00); only the
 * leading record fields are named. */
typedef struct {
    u8 pad0[2];
    u8 slot;
    u8 pad3;
    ItemWord item;
    u8 pad8[4];
    s32 value;
    u8 pad10[0x9C - 0x10];
} Packet;

/* Destination record (0xC bytes). */
typedef struct {
    s32 value;
    ItemWord item;
    u8 slot;
    u8 flag;
    u8 padA[2];
} Record;

extern Packet D_80147EA8;

extern s32 func_800CBD2C(void);
s32 func_800CBE44(void);
void func_800CBEBC(u8 a, u8 b);
void func_800CBF00(Packet *out);
s32 func_800CC6E4(Packet *s);
s32 func_800CC6F0(Packet *it);
extern void func_800CC248(void);

u8 func_800CCC44(u8 id, Record *records, u8 start, u8 count) {
    s32 ok = 1;
    s32 i;
    s32 end;

    if (func_800CBD2C() != 3) {
        ok = 0;
    } else {
        s32 linked = func_800CBE44() == 1;
        if (!linked) {
            ok = 0;
        }
    }
    func_800CBEBC(id, start);
    for (i = start, end = start + count; i < end; records++, i++) {
        if (ok) {
            func_800CBF00(&D_80147EA8);
            if (func_800CC6E4(&D_80147EA8) != 0) {
                count = i - start;
                break;
            }
            records->value = D_80147EA8.value;
            records->item = D_80147EA8.item;
            records->slot = D_80147EA8.slot;
            records->flag = func_800CC6F0(&D_80147EA8);
        } else {
            records->value = 0;
            records->item.kind = 0;
            records->slot = 0;
            records->flag = 0;
        }
    }
    func_800CC248();
    return count;
}
