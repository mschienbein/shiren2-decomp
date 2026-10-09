#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 pad0[0xC];
    u32 score_C;
    u32 time_10;
    u8 tail_14[0x88];
} Record800CC934;

extern u8 D_801476D8[];
extern Record800CC934 D_80147EA8;
s8 func_800CB510(u8 kind);
s32 func_800CBD2C(void);
s32 func_800CBE44(void);
void func_800CC16C(void);
void func_800CC248(void);
void func_80136964(const char *, ...);
void func_800CBFC0(u8 *base, u8 slot);
s32 func_800CBCF8(u8 index);
void func_800CB96C(u8 *src, Record800CC934 *dst);
void func_800CBC24(u8 *src, Record800CC934 *dst);
s32 func_800CC6E4(Record800CC934 *rec);
void func_800CB884(Record800CC934 *src, u8 *dst);
void func_800CB618(Record800CC934 *src, u8 *dst);
void *func_80032D94(void *, const void *, u32);
void func_800CC0E8(u8 *base, u8 slot);

void func_800CC934(u8 kind, Record800CC934 *rec) {
    s8 slot;
    s32 state;
    s32 rank;
    s32 i;
    s32 j;

    slot = func_800CB510(kind);
    if (slot == -1) {
        return;
    }
    state = func_800CBD2C();
    if (state == 1 || state == 2 || (func_800CBE44() ^ 1) != 0) {
        func_800CC16C();
        func_800CC248();
        state = func_800CBD2C();
    }
    if (state != 3) {
        func_80136964("Fatal Error Ranking Data Normaly Open\n");
    } else {
        func_800CBFC0(D_801476D8, slot);
        rank = -1;
        for (i = 0;; i++) {
            u8 *src;
            if (i >= 30) break;
            src = func_800CBCF8(i) + D_801476D8;
            if (i < 10) {
                func_800CB96C(src, &D_80147EA8);
            } else {
                func_800CBC24(src, &D_80147EA8);
            }
            if (func_800CC6E4(&D_80147EA8) || rec->score_C > D_80147EA8.score_C
                || (rec->score_C == D_80147EA8.score_C && rec->time_10 < D_80147EA8.time_10)) {
                rank = i;
                break;
            }
        }
        if (rank != -1) {
            u8 *dst;
            for (j = 29;;) {
                u8 *from;
                u8 *to;
                if (j-- <= rank) break;
                from = func_800CBCF8(j) + D_801476D8;
                to = func_800CBCF8(j + 1) + D_801476D8;
                if (j == 9) {
                    func_800CB96C(from, &D_80147EA8);
                    func_800CB884(&D_80147EA8, to);
                } else {
                    func_80032D94(to, from, (j < 9) ? 0x90 : 0x14);
                }
            }
            dst = func_800CBCF8(rank) + D_801476D8;
            if (rank < 10) {
                func_800CB618(rec, dst);
            } else {
                func_800CB884(rec, dst);
            }
            func_800CC0E8(D_801476D8, slot);
        }
    }
    func_800CC248();
}
